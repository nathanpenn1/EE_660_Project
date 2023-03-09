// SQL_Connection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//include the below additional libraries
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
//use the std namespace
using namespace std;



int main() {
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 1000
    //define handles and variables
    SQLHANDLE sqlConnectionHandle;
    SQLHANDLE sqlStatementHandle;
    SQLHANDLE sqlEventHandle;
    SQLCHAR retconstring[SQL_RETURN_CODE_LEN];

    //Declare incoming value
    int incoming_PersonID;
    char incoming_LastName[256];
    char incoming_FirstName[256];
    string incoming_Address;
    string incoming_City;

    // SQL Query Function
    char SQL_Select_Query[] = "SELECT TOP (1000) [PersonID], [LastName], [FirstName], [Address], [City] FROM[dbo].[Persons]";
    char SQL_Insert_Query[] = "INSERT INTO [dbo].[Persons] (PersonID, LastName, FirstName, Address, City) VALUES('9', 'Fred', 'Peter', '943 Glacier Blvd', 'Oxnard');";
    char SQL_Delete_Query[] = "DELETE from [dbo].[Persons] WHERE [LastName] = 'Fred';";    

    //initializations
    sqlConnectionHandle = NULL;
    sqlStatementHandle = NULL;
    //allocations
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEventHandle))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLSetEnvAttr(sqlEventHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEventHandle, &sqlConnectionHandle))
        goto COMPLETED;
    //output
    cout << "Attempting connection to SQL Server...";
    cout << "\n";
    switch (SQLDriverConnect(sqlConnectionHandle,
        NULL,
        (SQLCHAR*)"Driver={SQL Server};Server=tcp:sdsu-ee660.database.windows.net,1433;Database=SDSU_EE660;Uid=sdsu-ee660;Pwd=!QAZ1qaz;Encrypt=yes;TrustServerCertificate=no;Connection Timeout=30;",
        SQL_NTS,
        retconstring,
        1024,
        NULL,
        SQL_DRIVER_NOPROMPT)) {
    case SQL_SUCCESS:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_SUCCESS_WITH_INFO:
        cout << "Successfully connected to SQL Server";
        cout << "\n";
        break;
    case SQL_INVALID_HANDLE:
        cout << "Could not connect to SQL Server - Invalid Handle";
        cout << "\n";
        goto COMPLETED;
    case SQL_ERROR:
        cout << "Could not connect to SQL Server - SQL Server Error";
        cout << "\n";
        goto COMPLETED;
    default:
        break;
    }
    //if there is a problem connecting then exit application
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnectionHandle, &sqlStatementHandle))
        goto COMPLETED;
    //output
    cout << "\n";
    cout << "Resulting of Query:";
    cout << "\n";
    cout << "Person ID | Last Name | First Name | Address | City" << endl;
    cout << "\n";
    //if there is a problem executing the query then exit application
    //else display query result
    //Run SQL Select Function
    if (SQL_SUCCESS != SQLExecDirect(sqlStatementHandle, (SQLCHAR*)SQL_Select_Query, SQL_NTS)) {
        cout << "Error querying to SQL Server";
        cout << "\n";
        goto COMPLETED;
    }
    else {
        int PersonID;
        char LastName[256];
        char FirstName[256];
        char Address[256];
        char City[256];
        while (SQLFetch (sqlStatementHandle) == SQL_SUCCESS)
        {
            SQLGetData(sqlStatementHandle, 1, SQL_C_DEFAULT, &PersonID, sizeof(PersonID), NULL);
            SQLGetData(sqlStatementHandle, 2, SQL_C_DEFAULT, &LastName, sizeof(LastName), NULL);
            SQLGetData(sqlStatementHandle, 3, SQL_C_DEFAULT, &FirstName, sizeof(FirstName), NULL);
            SQLGetData(sqlStatementHandle, 4, SQL_C_DEFAULT, &Address, sizeof(Address), NULL);
            SQLGetData(sqlStatementHandle, 5, SQL_C_DEFAULT, &City, sizeof(City), NULL);

            cout << PersonID << ") " << LastName << " | " << FirstName << " | " << Address << " | " << City <<endl;

            cout <<"\n";
        }

        cout << "Type Person ID number:\n";
        cin >> incoming_PersonID;
        cout << "Type Last Name:\n";
        cin >> incoming_LastName;
        cout << "Type First Name:\n";
        cin >> incoming_FirstName;
        cin.ignore(); //Need this to clear out cin buffer
        cout << "Type Address:\n";
        getline(cin, incoming_Address); //getline will allow to input string with blank space from input
        cout << "Type City:\n";
        getline(cin, incoming_City);

        if (incoming_PersonID != PersonID)
        {
            goto INSERTSQL;
        }
        else
        {
            std::string string_Incoming_LastName = std::string(incoming_LastName);
            std::string string_LastName = std::string(LastName);
            if (string_Incoming_LastName != string_LastName)
            {
                goto INSERTSQL;
            }
            else
            {
                std::string string_Incoming_FirstName = std::string(incoming_FirstName);
                std::string string_FirstName = std::string(FirstName);
                if (string_Incoming_LastName != string_LastName)
                {
                    goto INSERTSQL;
                }
                else
                {
                    std::string string_Address = std::string(Address);
                    if (incoming_Address != string_Address)
                    {
                        goto INSERTSQL;
                    }
                    else
                    {
                        std::string string_City = std::string(City);
                        if (incoming_City != string_City)
                        {
                            cout << "Entry not matched with existing Database. New entry is inputing:\n";
                            goto INSERTSQL;
                        }
                        else
                        {
                            cout << "Input matched with existing data from Database. No data will be added onto the Database \n";
                            goto COMPLETED;
                        }
                    }
                }
            }
        }
    }

INSERTSQL:
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnectionHandle, &sqlStatementHandle))
        goto COMPLETED;
    if (SQL_SUCCESS != SQLExecDirect(sqlStatementHandle, (SQLCHAR*)SQL_Insert_Query, SQL_NTS)) {
        cout << "Error querying to SQL Server";
        cout << "\n";
        goto COMPLETED;
    }
    else {
        cout << "New entry has been inserted successfully\n";
    }

    //close connection and free resources
COMPLETED:
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStatementHandle);
    SQLDisconnect(sqlConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnectionHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEventHandle);
    //pause the console window - exit when key is pressed
    cout << "\nPress any key to exit...";
    getchar();
}

