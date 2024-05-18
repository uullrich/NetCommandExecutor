<!-- Introduction -->

# Remote command execution library
With this library you can create a cross platform tcp server and a command execution interface. The server is able to receive the follwing commands:
- GET &lt;endpoint&gt;
  <br>Should be used for single line responses.
- GET-DATA &lt;endpoint&gt;
  <br>Should be used for multiple line responses or binary data exchange.
- SET &lt;endpoint&gt;=&lt;value>
  <br>Should be used to set a single value.
- SET-DATA &lt;endpoint&gt;=&lt;expectedLength&gt;
  <br>Should be used to transmit larger amount of data. After the SET-DATA command the server expects the &lt;expectedLength&gt; amount of data.

<!-- Usage Example -->
## Usage example
  A usage example with a demo implementation can be found <a href="https://github.com/uullrich/NCE_Server_Demo">here</a>.

<!-- TechStack -->

## Tech Stack
<p align="left">
  <a href="https://skillicons.dev">
    <img src="https://skillicons.dev/icons?i=cpp,cmake,clion&perline=13" />
  </a>
</p>

## Command implementation
Commands can be implemented with the CommandExecutionInterface. 
A CommandExecutionInterface consists of an endpoint and callbacks for GET, GET-DATA, SET, SET-DATA commands.\
An endpoint is able to consist of several parts e.g. *device/&ast;/status*

You can use brace initialization and lambdas for a clean command implementation.
  ```
  CommandExecutionInterface cei = {
    {"device/*/status"},
    [](Command &command){
      //SET Callback
    },
    [](Command &command){
      //GET Callback
    },
    [](Command &command){
      //SET-DATA Callback
    },
    [](Command &command){
      //GET-DATA Callback
    }
  }
  ```
Another way to set instance methods as handlers is to use the fluent interface which is provided by the CommandExectionInterface.
  ```
    std::vector<CommandExecutionInterface> executionInterfaces;
    //The order is important!
    executionInterfaces << (CommandExecutionInterface(this) << Endpoint{"test"} << &ClassName::setHandler << &ClassName::getHandler << &ClassName::setDataHandler << &ClassName::getDataHandler);

    //For unused handlers use NO_RESPONSE
    executionInterfaces << (CommandExecutionInterface(this) << Endpoint{"another_test"} << NO_RESPONSE::SET << &ClassName::getHandler << NO_RESPONSE::SET_DATA << &ClassName::getDataHandler);
  ```

## Dependencies
The only build dependency is the boost library in version > 1.70.0.

## CMake usage example
- Compiler with C++20 support
- Go to your projects root directory
- ``mkdir lib``
- ``cd lib``
- Add this library as git submodule: ``git submodule add git@github.com:uullrich/NetCommandExecutor.git``
- Add the following parts to your *CMakeLists.txt*:

```
//...
add_subdirectory(libs/NetCommandExecutor)
include_directories(libs/NetCommandExecutor/include)
//...
target_link_libraries(YourProject NetCommandExecutor)
```
