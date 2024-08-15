#include <ntifs.h>

extern "C" {
    NTKERNELAPI NTSTATUS IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction);
    NTKERNELAPI NTSTATUS MmCopyVirtualMemory(PEPROCESS SourceProcess, PVOID SourceAddress, PEPROCESS TargetProcess, PVOID TargetAddress, SIZE_T BufferSize, KPROCESSOR_MODE PreviousMode, PSIZE_T ReturnSize);
   /* NTSTATUS MmCopyMemory(
        PEPROCESS SourceProcess,
        PVOID SourceAddress,
        PEPROCESS TargetProcess,
        PVOID TargetAddress,
        SIZE_T BufferSize,
        KPROCESSOR_MODE PreviousMode,
        PSIZE_T ReturnSize
    );*/
}

#define IOCTL_PRINT_MESSAGE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x18EA, METHOD_BUFFERED, FILE_SPECIAL_ACCESS); //Like IPC
constexpr ULONG init_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x18EB, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
constexpr ULONG read_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x18EC, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);
constexpr ULONG write_code = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x18ED, METHOD_BUFFERED, FILE_SPECIAL_ACCESS);

struct info_t {
    HANDLE target_pid = 0;
    void* target_address = 0x0;
    void* buffer_address = 0x0;
    SIZE_T size = 0;
    SIZE_T return_size = 0;
};

NTSTATUS ctl_io(PDEVICE_OBJECT device_obj, PIRP irp) {
    UNREFERENCED_PARAMETER(device_obj);

    static PEPROCESS s_target_process = nullptr;

    auto stack = IoGetCurrentIrpStackLocation(irp);
    auto buffer = (info_t*)irp->AssociatedIrp.SystemBuffer;

    if (buffer && stack->Parameters.DeviceIoControl.InputBufferLength >= sizeof(info_t)) {
        switch (stack->Parameters.DeviceIoControl.IoControlCode) {
        case init_code:
            PsLookupProcessByProcessId(buffer->target_pid, &s_target_process);
            break;
        case read_code:
            if (s_target_process) {
                MmCopyVirtualMemory(s_target_process, buffer->target_address, PsGetCurrentProcess(), buffer->buffer_address, buffer->size, KernelMode, &buffer->return_size);
            }
            break;
        case write_code:
            if (s_target_process) {
                MmCopyVirtualMemory(PsGetCurrentProcess(), buffer->buffer_address, s_target_process, buffer->target_address, buffer->size, KernelMode, &buffer->return_size);
            }
            break;
        default:
            irp->IoStatus.Status = STATUS_INVALID_DEVICE_REQUEST;
            break;
        }
    }
    else {
        irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
    }

    irp->IoStatus.Information = sizeof(info_t);
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return irp->IoStatus.Status;
}

NTSTATUS unsupported_io(PDEVICE_OBJECT device_obj, PIRP irp) {
    UNREFERENCED_PARAMETER(device_obj);

    irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return irp->IoStatus.Status;
}

NTSTATUS create_io(PDEVICE_OBJECT device_obj, PIRP irp) {
    UNREFERENCED_PARAMETER(device_obj);

    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return irp->IoStatus.Status;
}

NTSTATUS close_io(PDEVICE_OBJECT device_obj, PIRP irp) {
    UNREFERENCED_PARAMETER(device_obj);

    IoCompleteRequest(irp, IO_NO_INCREMENT);
    return irp->IoStatus.Status;
}

NTSTATUS real_main(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registery_path) {
    UNREFERENCED_PARAMETER(registery_path);

    UNICODE_STRING dev_name, sym_link;
    PDEVICE_OBJECT dev_obj;

    RtlInitUnicodeString(&dev_name, L"\\Device\\animepatrol");
    auto status = IoCreateDevice(driver_obj, 0, &dev_name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &dev_obj);
    if (status != STATUS_SUCCESS) return status;

    RtlInitUnicodeString(&sym_link, L"\\DosDevices\\animepatrol");
    status = IoCreateSymbolicLink(&sym_link, &dev_name);
    if (status != STATUS_SUCCESS) return status;

    SetFlag(dev_obj->Flags, DO_BUFFERED_IO);

    for (int t = 0; t <= IRP_MJ_MAXIMUM_FUNCTION; t++)
        driver_obj->MajorFunction[t] = unsupported_io;

    driver_obj->MajorFunction[IRP_MJ_CREATE] = create_io;
    driver_obj->MajorFunction[IRP_MJ_CLOSE] = close_io;
    driver_obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ctl_io;
    driver_obj->DriverUnload = NULL;

    ClearFlag(dev_obj->Flags, DO_DEVICE_INITIALIZING);
    return status;
}

extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT driver_obj, PUNICODE_STRING registery_path) {
    UNREFERENCED_PARAMETER(driver_obj);
    UNREFERENCED_PARAMETER(registery_path);

    UNICODE_STRING  drv_name;
    RtlInitUnicodeString(&drv_name, L"\\Driver\\animepatrol");
    IoCreateDriver(&drv_name, &real_main);

    return STATUS_SUCCESS;
}
