Requires python 3.6+ to build and configure

## Development

### VS Code 
- install cmake-tools, clangd, clang-format (set clang-format path to latest)
- symlink compile_commands.json from build folder to root folder
- install [Trigger task on save] and create this VS Code task, replace with path to build target folder
    ```json
    {
    "tasks": [
        {
            "label": "Build tests",
            "type": "shell",
            "presentation": {
              "echo": false,
              "reveal": "silent",
              "focus": false,
              "panel": "shared",
              "showReuseMessage": false,
              "clear": true
            },
            "group": {
                "kind": "build",
                "isDefault": true
              },
            "command": "cmake --build c:/dev/rara/build/default-debug --target tests",
            "problemMatcher": []
          }
    ],
    "version": "2.0.0"
    }
    ```

#### C++ in jupyter notebook

see https://github.com/jupyter-xeus/xeus-cling

and then https://code.visualstudio.com/docs/datascience/jupyter-notebooks#_connect-to-a-remote-jupyter-server




## Docs

Requires doxygen installed

> pip instal -r requirements.txt