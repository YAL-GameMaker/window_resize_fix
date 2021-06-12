{
    "id": "605ef5b0-6ed4-438b-a9d3-6428848f4629",
    "modelName": "GMExtension",
    "mvc": "1.2",
    "name": "window_resize_fix",
    "IncludedResources": [
        
    ],
    "androidPermissions": [
        
    ],
    "androidProps": true,
    "androidactivityinject": "",
    "androidclassname": "",
    "androidinject": "",
    "androidmanifestinject": "",
    "androidsourcedir": "",
    "author": "",
    "classname": "",
    "copyToTargets": 113497714299118,
    "date": "2019-34-12 01:12:29",
    "description": "",
    "exportToGame": true,
    "extensionName": "",
    "files": [
        {
            "id": "bac80d6c-a2e2-445c-9d9d-6141adb131a5",
            "modelName": "GMExtensionFile",
            "mvc": "1.0",
            "ProxyFiles": [
                
            ],
            "constants": [
                {
                    "id": "2d3d2066-7f71-a42f-062c-ad1928178850",
                    "modelName": "GMExtensionConstant",
                    "mvc": "1.0",
                    "constantName": "window_resize_fix_mode_copy",
                    "hidden": false,
                    "value": "0"
                },
                {
                    "id": "e843209f-af12-9ed9-c010-88b96067e4e0",
                    "modelName": "GMExtensionConstant",
                    "mvc": "1.0",
                    "constantName": "window_resize_fix_mode_stretch_aspect",
                    "hidden": false,
                    "value": "1"
                },
                {
                    "id": "4a7834e5-61f6-f78f-6eab-b55a68446fce",
                    "modelName": "GMExtensionConstant",
                    "mvc": "1.0",
                    "constantName": "window_resize_fix_mode_stretch_full",
                    "hidden": false,
                    "value": "2"
                }
            ],
            "copyToTargets": 9223372036854775807,
            "filename": "window_resize_fix.dll",
            "final": "",
            "functions": [
                {
                    "id": "78687533-1917-c27a-8ea4-16a2982466b5",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 0,
                    "args": [
                        
                    ],
                    "externalName": "window_resize_fix_reset",
                    "help": "",
                    "hidden": true,
                    "kind": 11,
                    "name": "window_resize_fix_reset",
                    "returnType": 2
                },
                {
                    "id": "7915d444-ccdc-6a52-7f37-667dc79f4870",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 4,
                    "args": [
                        1,
                        1,
                        1,
                        2
                    ],
                    "externalName": "window_resize_fix_init_raw",
                    "help": "",
                    "hidden": true,
                    "kind": 11,
                    "name": "window_resize_fix_init_raw",
                    "returnType": 2
                },
                {
                    "id": "3c2c3177-c4ca-1f3e-173d-16a29653cc11",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 0,
                    "args": [
                        
                    ],
                    "externalName": "window_resize_fix_get_just_resized",
                    "help": "window_resize_fix_get_just_resized()->bool",
                    "hidden": false,
                    "kind": 1,
                    "name": "window_resize_fix_get_just_resized",
                    "returnType": 2
                },
                {
                    "id": "c3d3ce88-4c42-97c1-4268-3480b9dbee3e",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 0,
                    "args": [
                        
                    ],
                    "externalName": "window_resize_fix_get_enabled",
                    "help": "window_resize_fix_get_enabled()->bool",
                    "hidden": false,
                    "kind": 1,
                    "name": "window_resize_fix_get_enabled",
                    "returnType": 2
                },
                {
                    "id": "87978acc-808e-5b85-e8c2-16a29953885a",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 1,
                    "args": [
                        2
                    ],
                    "externalName": "window_resize_fix_set_enabled",
                    "help": "window_resize_fix_set_enabled(enable) : (enable:bool)",
                    "hidden": false,
                    "kind": 1,
                    "name": "window_resize_fix_set_enabled",
                    "returnType": 2
                },
                {
                    "id": "ac26a4b8-0c94-0d42-b6b7-56c6ac3d57d1",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 2,
                    "args": [
                        2,
                        2
                    ],
                    "externalName": "window_resize_fix_set_aspect_ratio",
                    "help": "window_resize_fix_set_aspect_ratio(width, height) : (width:number, height:number)",
                    "hidden": false,
                    "kind": 1,
                    "name": "window_resize_fix_set_aspect_ratio",
                    "returnType": 2
                },
                {
                    "id": "fb37fd88-0efe-11e7-262a-b5426be6d1d6",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 1,
                    "args": [
                        2
                    ],
                    "externalName": "window_resize_fix_set_mode",
                    "help": "window_resize_fix_set_mode(mode)",
                    "hidden": false,
                    "kind": 1,
                    "name": "window_resize_fix_set_mode",
                    "returnType": 2
                }
            ],
            "init": "",
            "kind": 1,
            "order": [
                
            ],
            "origname": "extensions\\window_resize_fix.dll",
            "uncompress": false
        },
        {
            "id": "7cc73678-e3b5-432b-8372-a1d5779ceb4b",
            "modelName": "GMExtensionFile",
            "mvc": "1.0",
            "ProxyFiles": [
                
            ],
            "constants": [
                
            ],
            "copyToTargets": 9223372036854775807,
            "filename": "window_resize_fix.gml",
            "final": "",
            "functions": [
                {
                    "id": "39a3104c-f4bf-813b-7caa-0a30d8bc6242",
                    "modelName": "GMExtensionFunction",
                    "mvc": "1.0",
                    "argCount": 0,
                    "args": [
                        
                    ],
                    "externalName": "window_resize_fix_init",
                    "help": "window_resize_fix_init()->bool",
                    "hidden": false,
                    "kind": 2,
                    "name": "window_resize_fix_init",
                    "returnType": 2
                }
            ],
            "init": "",
            "kind": 2,
            "order": [
                
            ],
            "origname": "extensions\\gml.gml",
            "uncompress": false
        }
    ],
    "gradleinject": "",
    "helpfile": "",
    "installdir": "",
    "iosProps": true,
    "iosSystemFrameworkEntries": [
        
    ],
    "iosThirdPartyFrameworkEntries": [
        
    ],
    "iosdelegatename": "",
    "iosplistinject": "",
    "license": "Proprietary",
    "maccompilerflags": "",
    "maclinkerflags": "",
    "macsourcedir": "",
    "options": null,
    "optionsFile": "options.json",
    "packageID": "",
    "productID": "",
    "sourcedir": "",
    "supportedTargets": 113497714299118,
    "tvosProps": false,
    "tvosSystemFrameworkEntries": [
        
    ],
    "tvosThirdPartyFrameworkEntries": [
        
    ],
    "tvosclassname": "",
    "tvosdelegatename": "",
    "tvosmaccompilerflags": "",
    "tvosmaclinkerflags": "",
    "tvosplistinject": "",
    "version": "1.0.0"
}