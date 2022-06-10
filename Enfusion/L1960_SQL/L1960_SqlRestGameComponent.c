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
		PrintFormat(">>%1", jas.AsString());
		if (!jas.IsEmpty())
		{
			SCR_HintManagerComponent.ShowCustomHint(jas.AsString(), "SQL REST", 12);
		}
		else
			SCR_HintManagerComponent.ShowCustomHint("Null?!", "SQL REST", 12);
	}
	
	void test()
	{
		Sleep(3000);
		
		/* getting the current timestamp from the DB as a test */
		string sql = " SELECT CURRENT_TIMESTAMP() as time";
		
		/* synchronous example - must be executed a new thread */
		PrintFormat(">%1", L1960_SQLREST.Query(0,sql).AsString());
		
		Sleep(1000);
		
		/* asynchronous example - can run in the main thread */
		auto tmp = new L1960_SQLREST();
		tmp.Insert(print_json);  // receive the result in the `print_json` function which takes a `L1960_SQLQuery` object
		tmp.Query_async(sql); 
		
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