# A Basic Kernel Driver R/W with IOCTL

Pipe We Using is "animepatrol"
```cpp
auto driver = new driver_manager("\\\\.\\animepatrol", process_id);
```

### Info to Set-up
```ascii
C++ Language Standard : ISO C++20 Standard (/std:c++20)
```

```
Driver Project ("Kernel") : 
Configuration Properties -> Linker -> Advanced -> FxDriverENtry -> "DriverEntry",
Configuration Properties -> C/C++ -> Code Generation -> Security Check -> "Disable Security Check (/GS-)"
```
<img src="https://github.com/RealSyferX/Kernel-R-W-With-Usermode-IOCTL-/blob/master/img/KernEntry.jpg?raw=true" alt="Kernel">
<img src="https://github.com/RealSyferX/Kernel-R-W-With-Usermode-IOCTL-/blob/master/img/KernSec.jpg?raw=true" alt="Kernel">


```
UserMode Project ("Client") : Configuration Properties -> Advanced -> Character Set -> "Not Set"
```
<img src="https://github.com/RealSyferX/Kernel-R-W-With-Usermode-IOCTL-/blob/master/img/charCLient.jpg?raw=true" alt="Kernel">

## TODO : 
```
Get a Module Handler from Kernel Driver instead of UserMode
Load A Kernel Driver From BIOS/UEFI (AMD/INTEL)
```