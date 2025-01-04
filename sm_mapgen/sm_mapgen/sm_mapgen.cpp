#include <iostream>
#include <string>
#include <sqlite3.h>
#include <vector>

#include <lua.hpp> 

using namespace std;



int getSQLiteSeed(const char* dbFile) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    // Open the SQLite database
    if (sqlite3_open(dbFile, &db) != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }

    const char* sql = "SELECT seed FROM Game;";  
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return 0;
    }

    int seed = 0;

    // Execute the query and retrieve results
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        seed = sqlite3_column_int(stmt, 0);

        cout << seed << endl;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt)
        ;
    sqlite3_close(db);
    return seed;
}


int main(int argc, char* argv[]) {

    string filepath = "";

    if (argc < 1) {
        filepath = argv[1];
    }
    else {
        cout << "Enter path of survival save  (Save/Survival/EXAMPLE.db):" << endl;
        cin >> filepath;
    }

    const int seed = getSQLiteSeed(filepath.c_str());
    if (seed == 0) {
        cerr << "Failed to get game seed";
        return 1;
    }


    // Initialize the Lua interpreter
    lua_State* L = luaL_newstate();
    if (L == nullptr) {
        cerr << "Failed to create Lua state!" << endl;
        return -1;
    }

    // Open Lua libraries
    luaL_openlibs(L);

    // Load and execute a Lua script
    const char* scriptFile = "../../main.lua"; 
    if (luaL_dofile(L, scriptFile) != LUA_OK) {
        cerr << "Error running script: " << lua_tostring(L, -1) << endl;
        lua_close(L);
        return -1;
    }

    // Call Lua function (example: call a function from the script)
    lua_getglobal(L, "luaFunction");  // Assuming your Lua script defines a function called "luaFunction"
    if (lua_isfunction(L, -1)) {
        lua_pushnumber(L, seed); // add seed to stack
        // Call the Lua function
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            cerr << "Error calling Lua function: " << lua_tostring(L, -1) << endl;
        }
    }

    // Clean up Lua state
    lua_close(L);
    return 0;
}
