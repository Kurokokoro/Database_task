#include "sqlite3.h"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>

using namespace std;

// dodaje do nazwy bazy rozszerzenie .db jeœli barakuje
string Add_extension(string dbase_name, string extension){
    if (dbase_name.length() >= extension.length()){
        if ( 0 == dbase_name.compare(dbase_name.length() - extension.length(), extension.length(), extension))
            return dbase_name;
        else
            return (dbase_name + extension);
    }
    return (dbase_name + extension);
}

// wypisuje dane bazy formie tabeli
static int Callback(void *data, int argc, char **argv, char **ColName){
    int offset = 0;
    for(int i = 0; i<argc; i++){
        if (0 < i && i < 3)
            offset = 20;
        else
            offset = 8;
        cout << setw(offset) << (argv[i] ? argv[i] : "NULL") << "|";
    }
    cout << endl;
    return 0;
}

// wykonuje polecenia na bazie
int Execute_statement(sqlite3* database, const char * statement){
    int response = 0;
    do{
        // prepare the statement
        if (SQLITE_OK != (response = sqlite3_exec(database, statement, Callback, 0, NULL)))
        {
            cout << "Failed to prepare insert: %d, %s\n" << response << sqlite3_errmsg(database) << endl;
            break;
        }
    } while(false);
    return response;
}

// wypisuje nazwy kolumn tabeli
void Get_column_names(sqlite3* database, const char *char_statement){
    sqlite3_stmt* query = NULL;
    sqlite3_prepare_v2(database, char_statement, -1, &query, NULL);
    int offset = 0;
    for (int i = 0; i < 4; i++){
        if (0 < i && i < 3)
            offset = 20;
        else
            offset = 8;
        cout << setw(offset) << sqlite3_column_name(query, i)  << "|";
    }
    cout << endl;
    cout.fill('-');
    cout << setw(9) << '|' << setw(21) << '|' << setw(21) << '|' << setw(9) << '|' << setw(3) << "-" << endl;
    cout.fill(' ');
    }

// wybór operacji do zastosowania na bazie
int Chose_action(sqlite3* database, string table_name, int response){
    string statement = "";
    const char* char_statement = "";
    string user_input = "";
    // pêtla z opcjami operacji do zastosowania na bazie
    while (true){
        cout << "Specify the operation, please. (print/insert/delete/custom/exit)" << endl;
        cin >> user_input;
        if (user_input == "print"){
            statement = "SELECT * FROM " + table_name;
            char_statement = statement.c_str();
            Get_column_names(database, char_statement);
            response += Execute_statement(database, char_statement);
            cout << endl;
        }
        if (user_input == "insert"){
            cout << "Insert data in format: (IMIE, NAZWISKO, WIEK), e.g. ('Jan', 'Kowalski', 22)" << endl;
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(cin, user_input);
            statement = "INSERT INTO " + table_name + " (IMIE, NAZWISKO, WIEK) VALUES " + user_input + ";";
            char_statement = statement.c_str();
            response += Execute_statement(database, char_statement);
            cout << "Record inserted into database" << endl;
        }
        if (user_input == "delete"){
            cout << "Insert data to delete in format: COLUMN_NAME=VALUE, e.g. NAME='Jan'" << endl;
            cin >> user_input;
            statement = "DELETE FROM " + table_name + " WHERE " + user_input;
            char_statement = statement.c_str();
            response += Execute_statement(database, char_statement);
            cout << "Records deleted form database" << endl;
        }
        if (user_input == "custom"){
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(cin, statement);
            char_statement = statement.c_str();
            if (statement.find("SELECT") == 0)
                Get_column_names(database, char_statement);
            response += Execute_statement(database, char_statement);
            cout << endl;
        }
        if (user_input == "exit"){
            break;
        }
    }
    return response;
}

// zajmuje siê po³¹czeniem z baz¹ danych i wykonywaniem operacji na niej
int Interact_with_dbase(string dbase_name, string table_name){
    dbase_name = Add_extension(dbase_name, ".db");
    const char* dbase_updated_name = dbase_name.c_str();
    sqlite3* database = NULL;
    int response = 0;
    do
    {
        if (SQLITE_OK != (response = sqlite3_initialize()))
        {
            cout << "Failed to initialize library: " << response << endl;
            break;
        }
        // ³¹czenie z baz¹ danych
        if (SQLITE_OK != (response = sqlite3_open_v2(dbase_updated_name, &database, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL)))
        {
            cout << "Failed to connect to database," << response << endl;
            break;
        }
        string statement = "CREATE TABLE IF NOT EXISTS " + table_name + "(\
                                    ID INTEGER PRIMARY KEY AUTOINCREMENT,\
                                    IMIE TEXT NOT NULL,\
                                    NAZWISKO TEXT NOT NULL,\
                                    WIEK NUMERIC(2) NOT NULL\
                                    );";
        const char* char_statement = statement.c_str();
        if (SQLITE_OK != (response = Execute_statement(database, char_statement)))
        {
            cout << "Failed to execute creating database," << response << endl;
            break;
        }
        if (SQLITE_OK != (response = Chose_action(database, table_name, response)))
        {
            cout << "Failed to execute command," << response << endl;
            break;
        }
    } while (false);
    if (NULL != database) sqlite3_close(database);
    sqlite3_shutdown();
    return response;
}
