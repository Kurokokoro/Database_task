#include <iostream>
#include "Database_management.h"


using namespace std;

int main()
{   string database_name = "";
    string table_name = "Personal_data";
    cout << "Please, specify database name: " << endl;
    cin >> database_name;
    cout << "Table name is: " << table_name << endl;
    int x = Interact_with_dbase(database_name, table_name);
    return x;
}
