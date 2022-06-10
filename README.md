# ARMA Reforger SQL over REST Api
This project uses the REST Api to query a PHP web page (also part of the project) to query a SQL database.
It can be used to grant each client (proxy) access to the database via the Webserver or route all traffic through the ARMA backend perform only local queries to the DB.
The login, passwork and DB name are passed to the PHP script via POST parameters.
The HTTP responds is parsed as JSON and is decoded using the enfusion JsonApiStruct. 
## Getting started
Just add the contents of the `Enfusion` folder into your ARMA Reforger Workbench project and host the `restAPI.php` on any Webserver.
The Webserver must be configured such that it has access to the DB - preferable local-access to avoid unsafe traffic through the internet.
Arma must have access to the Webserver - either via localhost or publicly in the internet.
## Usage
You can perform synchronous:

    L1960_SQLREST.Query(0,"SELECT CURRENT_TIMESTAMP() as time")

and asynchronous queries:

    auto tmp = new L1960_SQLREST();
    tmp.Insert(func);  // receive the result in the func function which takes a L1960_SQLQuery object
    tmp.Query_async(sql); 
    
The result is parsed into a `L1960_SQLQuery` object where you can scan over all rows using a loop and printing the `Name` column:

    foreach (auto ele : sqlQuery)
    {
      Print(ele.GetString('Name'))
    }

## Contributing
Any help or ideas for improvement are very welcome. Please feel free to point out any errors in the code or improvements of the code in general.
