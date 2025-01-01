#include <iostream>
#include <string>

#include <lua.hpp> 

using namespace std;

/* 
TODO
- make global classes for lua to utilise

sm.terrainData.load() -- if I can get this i wont actually need to mix it with lua


- json dump the g_cellData object 
- json dump in the format expected by the mapping tool


*/


void loadSQL(string filepath) {
    cout << filepath;
}


int main(int argc, char* argv[]) {

    string filepath = "";

    if (argc < 1) {
        filepath = argv[1];
    }
    else {
        cin >> filepath;
    }

    loadSQL(filepath);


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
        std::cerr << "Error running script: " << lua_tostring(L, -1) << std::endl;
        lua_close(L);
        return -1;
    }

    // Call Lua function (example: call a function from the script)
    lua_getglobal(L, "luaFunction");  // Assuming your Lua script defines a function called "luaFunction"
    if (lua_isfunction(L, -1)) {
        // Call the Lua function
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            std::cerr << "Error calling Lua function: " << lua_tostring(L, -1) << std::endl;
        }
    }

    // Clean up Lua state
    lua_close(L);
    return 0;
}
