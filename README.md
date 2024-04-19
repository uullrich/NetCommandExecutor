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

## Command implementation
Commands can be implemented with the CommandExecutionInterface. 
A CommandExecutionInterface consists of an endpoint and callbacks for GET, GET-DATA, SET, SET-DATA commands.\
An endpoint is able to consist of several parts e.g. *device/&ast;/status*

You can use brace initialization and lambdas for a clean command creation.
  ```
  CommandExecutionInterface cei = {
    {"device/*/status"},
    [](Command & command){
      //SET Callback
    },
    [](Command & command){
      //GET Callback
    },
    [](Command & command){
      //SET-DATA Callback
    },
    [](Command & command){
      //GET-DATA Callback
    }
  }
  ```

## Dependencies
The only dependency is the boost library.