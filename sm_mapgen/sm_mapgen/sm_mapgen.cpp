#include <iostream>
#include <lua.hpp> 

int main() {
    // Initialize the Lua interpreter
    lua_State* L = luaL_newstate();
    if (L == nullptr) {
        std::cerr << "Failed to create Lua state!" << std::endl;
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
