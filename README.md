# A Basic Kernel Driver R/W with IOCTL

Pipe We Are Using: `"animepatrol"`
```cpp
auto driver = new driver_manager("\\\\.\\animepatrol", process_id);
```

### Setup Information

#### C++ Language Standard
```
ISO C++20 Standard (/std:c++20)
```

#### Driver Project ("Kernel")
```
Configuration Properties -> Linker -> Advanced -> Entry Point -> "DriverEntry"
Configuration Properties -> C/C++ -> Code Generation -> Security Check -> "Disable Security Check (/GS-)"
```
<img src="https://github.com/RealSyferX/Kernel-R-W-With-Usermode-IOCTL-/blob/master/img/KernEntry.jpg?raw=true" alt="Driver Entry Configuration">
<img src="https://github.com/RealSyferX/Kernel-R-W-With-Usermode-IOCTL-/blob/master/img/KernSec.jpg?raw=true" alt="Security Check Configuration">

#### UserMode Project ("Client")
```
Configuration Properties -> Advanced -> Character Set -> "Not Set"
```
<img src="https://github.com/RealSyferX/Kernel-R-W-With-Usermode-IOCTL-/blob/master/img/charCLient.jpg?raw=true" alt="Character Set Configuration">

## TODO:
- Get a Module Handler from the Kernel Driver instead of UserMode
- Load a Kernel Driver from BIOS/UEFI (AMD/INTEL)
