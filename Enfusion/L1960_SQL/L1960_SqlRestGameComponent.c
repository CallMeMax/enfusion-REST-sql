[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Adds survival aspects.", color: "0 0 255 255")]
class L1960_SqlRestGameComponentClass: SCR_BaseGameModeComponentClass
{
};



class L1960_SqlRestGameComponent : SCR_BaseGameModeComponent {
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//--- Public functions
	////////////////////////////////////////////////////////////////////////////////////////////////////
	
	void print_json(L1960_SQLQuery jas)
	{
		if (!jas.IsEmpty())
		{
			SCR_HintManagerComponent.ShowCustomHint(jas.AsString(), "SQL REST", 12);
			PrintFormat(">>%1", jas.AsString());
		}
		else
			SCR_HintManagerComponent.ShowCustomHint("Null?! " + jas.m_sError, "SQL REST", 12);
	}
	
	void test()
	{
		Sleep(3000);
		
		/* getting the current timestamp from the DB as a test */
		string sql = "SELECT CURRENT_TIMESTAMP() as time";
		
		/* synchronous example - must be executed a new thread */
		auto query = L1960_SQL.Query(0,sql);
		if (query.IsEmpty())
			PrintFormat(">%1", query.AsString());
		else
			Print("sync query failed! " + query.m_sError, LogLevel.ERROR);
		
		Sleep(1000);
		
		/* asynchronous example - can run in the main thread */
		auto tmp = new L1960_SQL();
		tmp.Insert(print_json);  // receive the result in the `print_json` function which takes a `L1960_SQLQuery` object
		if(!tmp.Query_async(sql))
			Print("async query failed!", LogLevel.ERROR);
		
		Sleep(1000);  // keep tmp alive until 2nd query is complete
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//--- Default functions
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/*!
	Test the implementation by calling test() in a new thread.
	*/
	override void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		thread test();
	}
}