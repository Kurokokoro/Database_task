#include "Database_management.cpp"

using namespace std;

// dodaje do nazwy bazy rozszerzenie .db jeœli barakuje
string Add_extension(string dbase_name, string extension);
// wypisuje dane bazy formie tabeli
static int Callback(void *data, int argc, char **argv, char **ColName);
// wykonuje polecenia na bazie
int Execute_statement(sqlite3* database, const char * statement);
// wypisuje nazwy kolumn tabeli
void Get_column_names(sqlite3* database, const char *char_statement);
// wybór operacji do zastosowania na bazie
int Chose_action(sqlite3* database, string table_name, int response);
// zajmuje siê po³¹czeniem z baz¹ danych i wykonywaniem operacji na niej
int Interact_with_dbase(string base_name);
