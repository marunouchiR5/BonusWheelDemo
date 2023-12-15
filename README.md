# BonusWheelDemo

This repository contains a Cocos2d-x v4 project that demonstrates a bonus wheel game feature.

## Setting Up the Working Environment

1. **Prerequisites**: Ensure all prerequisites for Cocos2d-x v4 are met as per [Cocos2d-x prerequisites](https://docs.cocos2d-x.org/cocos2d-x/v4/en/installation/prerequisites.html).

2. **Create a New Project**:
```
cocos new FirstGame -l cpp -p com.mycompany.mygame -d Games/.
```


3. **Initialize Git Repository**:
```
git init
```

4. **Add Remote Repository**:
```
git remote add origin https://github.com/marunouchiR5/BonusWheelDemo.git
```

5. **Fetch Data**:
```
git fetch
```

6. **Delete Files**:
- `Classes/AppDelegate.cpp`
- `Classes/AppDelegate.h`
- `Classes/HelloWorldScene.cpp`
- `Classes/HelloWorldScene.h`
- `Resources/CloseNormal.png`
- `Resources/CloseSelected.png`
- `Resources/HelloWorld.png`

7. **Checkout Main Branch**:
```
git checkout main
```

### Windows Setup

8. **Navigate to `proj.win32`**:
```
cd path/to/proj.win32
cmake .. -G "Visual Studio 17 2022" -A Win32
```

9. **Open Project in Visual Studio**:
Double-click the `.sln` file.

10. **Remove ALL_BUILD**:
Remove `ALL_BUILD` under `CMakePredefinedTargets` in Visual Studio.

11. **Run Project**:
Click "Run Local Windows Debugger" in Visual Studio.




