# A Basic Kernel Driver R/W with IOCTL

Pipe We Using is "animepatrol"
```cpp
auto driver = new driver_manager("\\\\.\\animepatrol", process_id);
```

### Info to Set-up
```ascii
C++ Language Standard : ISO C++20 Standard (/std:c++20)
Driver Project ("Kernel") : Configuration Properties -> Linker -> Advanced -> FxDriverENtry -> "DriverEntry"
UserMode Project ("Client") : Configuration Properties -> Advanced -> Character Set -> "Not Set"
```

## TODO : 
```
Get a Module Handler from Kernel Driver instead of UserMode
Load A Kernel Driver From BIOS/UEFI (AMD/INTEL)
```
test