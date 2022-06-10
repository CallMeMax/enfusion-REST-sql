class L1960_SQLREST : ScriptInvoker
{
	
	private const static string username = "";
	private const static string dbname = "";
	private const static string password = "";
    private const static string server = "";
	
	static L1960_SQLQuery Query(int i, string request)
	{
		L1960_SQLRESTCallback cbx1 = new L1960_SQLRESTCallback();
		RestContext ctx = g_Game.GetRestApi().GetContext(server);
		
		i = 0;  // must be zero or the game/workbench crashes
		
		string data = "password="+password+"&dbname="+dbname+"&data="+request+"&username="+username;
		
		ctx.POST(cbx1, "restAPI.php", data);
		
		while (!cbx1.ready)
			Sleep(10);
		
		return cbx1.jas;
	}

	void Query_async(string request)
	{
		L1960_SQLRESTCallback cbx1 = new L1960_SQLRESTCallback(this);
		RestContext ctx = g_Game.GetRestApi().GetContext(server);
		
		string data = "password="+password+"&dbname="+dbname+"&data="+request+"&username="+username;
		
		ctx.POST(cbx1, "restAPI.php", data);
	}	
}



class L1960_SQLRESTCallback : RestCallback
{

	bool ready = false;
	ref L1960_SQLQuery jas;
	private L1960_SQLREST parent;

 	void L1960_SQLRESTCallback(L1960_SQLREST p = null)
	{
		parent = p;
	}

	override void OnError( int errorCode )
	{
		// override this with your implementation
		Print(" !!! OnError() ", LogLevel.WARNING);
		ready = true;
		parent.Invoke(new L1960_SQLQuery());
	};

	/**
	\brief Called in case request timed out or handled improperly (no error, no success, no data)
	*/
	override void OnTimeout()
	{
		Print(" !!! OnTimeout() ", LogLevel.WARNING);
		ready = true;
		parent.Invoke(new L1960_SQLQuery());
	};

	/**
	\brief Called when data arrived and/ or response processed successfully
	*/
	override void OnSuccess( string data, int dataSize )
	{
		jas = new L1960_SQLQuery(data);
		
		ready = true;

		if (parent)
			parent.Invoke(jas);
	};

	/**
	\brief Called when data arrived and/ or file created successfully
	*/
	override void OnFileCreated( string fileName, int dataSize )
	{
		Print(" !!! OnFileCreated() file=" + fileName + " size=" + dataSize, LogLevel.WARNING);
		ready = true;
		parent.Invoke(new L1960_SQLQuery());
	};

}