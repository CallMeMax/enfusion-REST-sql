class L1960_SQLRESTCallback : RestCallback
{

	private bool m_bReady = false;
	private ref L1960_SQLQuery m_queryResult;
	private L1960_SQL m_parent;

	/*!
	Receives reference to the SQL parent class or NULL.
	The parent is used to invoke the the callback if an asynchronous strategy is chosen. 
	*/	
 	void L1960_SQLRESTCallback(L1960_SQL p = null)
	{
		m_parent = p;
	}
	
	/*!
	Checks if the responds to the SQL query has been received and parsed.
	\return bool if callback has been triggered
	*/
	bool IsReady()
	{
		return m_bReady;
	}
	
	/*!
	Returns the result of the SQL query.
	\return L1960_SQLQuery the parsed SQL result
	*/
	L1960_SQLQuery GetResult()
	{
		return m_queryResult;
	}

	/**
	\brief Called in case an error occurred
	Stops waiting by setting m_bReady true and invokes callback with empty query.
	*/	
	override void OnError(int errorCode)
	{
		Print(" !!! OnError() ", LogLevel.WARNING);
		m_bReady = true;
		m_parent.Invoke(new L1960_SQLQuery());
	};

	/**
	\brief Called in case request timed out or handled improperly (no error, no success, no data)
	Stops waiting by setting m_bReady true and invokes callback with empty query.
	*/
	override void OnTimeout()
	{
		Print(" !!! OnTimeout() ", LogLevel.WARNING);
		m_bReady = true;
		m_parent.Invoke(new L1960_SQLQuery());
	};

	/**
	\brief Called when data arrived and/ or response processed successfully
	Stops waiting by setting m_bReady true and invokes callback.
	*/
	override void OnSuccess(string data, int dataSize)
	{
		m_queryResult = new L1960_SQLQuery(data);
		
		m_bReady = true;

		if (m_parent)
			m_parent.Invoke(m_queryResult);
	};

	/**
	\brief Called when data arrived and/ or file created successfully (should never be called?!)
	Stops waiting by setting m_bReady true and invokes callback with empty query.
	*/
	override void OnFileCreated(string fileName, int dataSize)
	{
		Print(" !!! OnFileCreated() file=" + fileName + " size=" + dataSize, LogLevel.WARNING);
		m_bReady = true;
		m_parent.Invoke(new L1960_SQLQuery());
	};

}

class L1960_SQLConfigJson : JsonApiStruct
{
	string username = "";
	string dbname = "";
	string password = "";
    string server = ""; 

	/*!
	Registers the four member variables as content of the Json config.
	*/	
	void L1960_SQLConfigJson()
	{
		RegV("username");
		RegV("dbname");
		RegV("password");	
		RegV("server");
	}

	/*!
	Returns the variables as string for debugging purposes
	*/	
	string AsString2()
	{
		return 	username+", "+dbname+", "+password+", "+server;
	}
}

class L1960_SQLConfig : DSSessionCallback
{
	
	ref L1960_SQLConfigJson m_configJson = new L1960_SQLConfigJson();
	private const string FILENAME = "restSQL";

	/*!
	Tries to read the config with the configuration of the DB and creates an empty config if non is present.
	*/
	void L1960_SQLConfig()
	{
		SessionStorage storage = GetGame().GetBackendApi().GetStorage();
		storage.AssignFileIDCallback(FILENAME, this);	
		
		load();
		
		save();
	}

	/*!
	Returns the content of the DB config as string for debug purposes
	*/
	string AsString()
	{
		return m_configJson.AsString();
	}
	
	/*!
	Loads config from json file.
	*/
	void load()
	{
		SessionStorage storage = GetGame().GetBackendApi().GetStorage();
		storage.LocalLoad(FILENAME);
	}
	
	/*!
	Saves config to json file.
	*/
	void save()
	{
		SessionStorage storage = GetGame().GetBackendApi().GetStorage();
		storage.LocalSave(FILENAME);
	}

	/**
	\brief Save event handling
	*/
	override void OnSaving(string fileName)
	{			
		m_configJson.Pack();
		GetGame().GetBackendApi().GetStorage().ProcessSave(m_configJson, fileName);
	}

	/**
	\brief Load event handling
	*/
	override void OnLoaded(string fileName)
	{		
		GetGame().GetBackendApi().GetStorage().ProcessLoad(m_configJson, fileName);
		m_configJson.Pack();
	}
}

class L1960_SQL : ScriptInvoker
{
	static ref L1960_SQLConfig cfg;
	
	/*!
	Performes synchronous SQL query to the DB specified by the json config.
	*/
	static L1960_SQLQuery Query(int i, string request)
	{			
		if (!cfg)
			cfg = new L1960_SQLConfig();
		L1960_SQLRESTCallback cbx1 = new L1960_SQLRESTCallback();
		RestContext ctx = g_Game.GetRestApi().GetContext(cfg.m_configJson.server);
		
		if (!ctx)
		{
			Print("RestSQL Error: config file does not contain valid server name `"+cfg.m_configJson.server+"`", LogLevel.FATAL);
			return null;
		}
		
		i = 0;  // must be zero or the game/workbench crashes
		
		string data = "password="+cfg.m_configJson.password+"&dbname="+cfg.m_configJson.dbname+"&data="+request+"&username="+cfg.m_configJson.username;
		
		ctx.POST(cbx1, "restAPI.php", data);
		
		while (!cbx1.IsReady())
			Sleep(10);
		
		return cbx1.GetResult();
	}

	/*!
	Performes asynchronous SQL query to the DB specified by the json config.
	*/
	bool Query_async(string request)
	{
		if (!cfg)
			cfg = new L1960_SQLConfig();
		L1960_SQLRESTCallback cbx1 = new L1960_SQLRESTCallback(this);
		RestContext ctx = g_Game.GetRestApi().GetContext(cfg.m_configJson.server);
		
		if (!ctx)
		{
			Print("RestSQL Error: config file does not contain valid server name `"+ cfg.m_configJson.server+"`", LogLevel.FATAL);
			return false;
		}
		
		string data = "password="+cfg.m_configJson.password+"&dbname="+cfg.m_configJson.dbname+"&data="+request+"&username="+cfg.m_configJson.username;
		
		ctx.POST(cbx1, "restAPI.php", data);
		
		return true;
	}
	
}