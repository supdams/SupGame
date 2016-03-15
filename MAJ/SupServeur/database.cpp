/**************
* File Name: database.cpp
* Author: G. J. Krafsig
* Date: June 28, 2007
* Purpose: implementation of the game database
**************/
#include "database.hpp"

/**************
* Purpose: default constructor, initialize class values
* Precondition: none
* Postcondition: none
**************/
database::database()
{
    //initilize the mysql fields
    sock = NULL;
    row = NULL;
    result = NULL;
    field = NULL;          
}

/**************
* Purpose: connect to the appropriate database
* Precondition: host, database, user name and password
* Postcondition: connected to that database
**************/
int database::openConnection(char *host, char *db, char *user, char *pass)
{
    if (sock) //already connected to another database
        disconnect(); //disconnect from that one
        
    //initilize the socket
    sock = mysql_init(0);
    
    //something went wrong with the socket
    if (!sock)
        return ERR201;
     
    //try connecting to the database  
    if (!mysql_real_connect(sock, host, user, pass, db, 0, NULL, 0))
        return ERR202;
    
    //successfully connected to the database    
    return SUCCESS;  
}

/**************
* Purpose: disconnect from the database
* Precondition: none
* Postcondition: socket closed
**************/
bool database::disconnect()
{
    if (sock) //they have a socket open
        mysql_close(sock);
    
    //release result data
    free(); 
    
    //database disconnected
    return true; 
}

/**************
* Purpose: free the results from the database query
* Precondition: none
* Postcondition: results no longer there
**************/
bool database::free()
{
    if (result)
    {
        mysql_free_result(result); 
        return true;
    }
    
    return false;
}

/**************
* Purpose: return the approprite error message
* Precondition: error code
* Postcondition: string with the error returned
**************/
char *dberror(int errorcode)
{
    
    //display the appropriate error message for this error    
    switch(errorcode)
    {
        case SUCCESS: return "SUCCESS";
            break;
            
        case ERR201: return "201 SOCKET ERROR: SOCKET FAILURE";
            break;
            
        case ERR202: return "202 CONNECTION ERROR: CANNOT ACCESS THE SERVER";
            break;
            
        case ERR203: return "203 DATABASE ERROR: QUERY FAILED";
            break;
    }
    
    return NULL; //no error, return null char
}

/**************
* Purpose: return the result set of the query
* Precondition: the query string
* Postcondition: result set returned (or null)
**************/
MYSQL_RES *database::query(char *query)
{        
    //query the database
    mysql_query(sock, query);
    
    //store the results
    result = mysql_store_result(sock);
    
  
if (result) {
    RowsReturned = mysql_num_rows( result );
} else {
    RowsReturned = 0;
}


    return result;
}

MYSQL_RES *database::query2(char *query)
{        
    //query the database
    mysql_query(sock, query);
    
    //store the results
    result = mysql_store_result(sock);
    
    return result;
    
}

long database::NumRows()
{
    return RowsReturned;
}

MYSQL_ROW database::GetRow()
{
    return mysql_fetch_row( result );
}

/**************
* Purpose: update the database no result returned
* Precondition: the query string
* Postcondition: false if failed, true if suceess
**************/
bool database::updateQuery(char *query)
{        
    if (!mysql_query(sock, query))
        return 0; //failed query
    else
        return 1; //successful query
}

/**************
* Purpose: return the result set of the query
* Precondition: the query string
* Postcondition: the FIRST result is returned (or null)
*                will not return multiple rows, only the first
**************/
char *database::stringQuery(char *query)
{
    //if old results exist, free them
    //free();
        
    //query the database
    mysql_query(sock,query);
    
    //store the results
    result = mysql_store_result(sock);
    
    if (!result)
        return NULL; //no results
    
    //fetch the row
    row = mysql_fetch_row(result);
    
    //store the result & convert it to a number
    char *stringResult = row[0];
    
    //free the results
    free();
    
    return stringResult;
    
}

/**************
* Purpose: return the result set of the query
* Precondition: the query string
* Postcondition: the FIRST result is returned (or null)
*                will not return multiple rows, only the first
**************/
int database::intQuery(char *query)
{        
    //query the database
    mysql_query(sock,query);
    
    //store the results
    result = mysql_store_result(sock);
    
    if (!result)
        return -1; //no results
    
    //fetch the row
    row = mysql_fetch_row(result);
    
    //store the result & convert it to a number
    int id = atoi(row[0]);
    
    //free the results
    free();
    
    return id; //return the id number
    
}

/**************
* Purpose: return the result set of the query
* Precondition: the query string
* Postcondition: the FIRST result is returned (or null)
*                will not return multiple rows, only the first
**************/
bool database::boolQuery(char *query)
{
    //if old results exist, free them
    //free();
        
    //query the database
    mysql_query(sock, query);
    
    //store the results
    result = mysql_store_result(sock);
    
    //fetch the row
    //row = mysql_fetch_row(result);
    
    return (bool)row[0];
    
}