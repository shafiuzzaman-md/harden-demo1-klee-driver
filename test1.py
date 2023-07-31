import re

# Define the regular expression pattern to find the statements containing gBS->
gBS_pattern = r'(gBS->[^;]*;)'

# Example usage:
content = '''
    Status = gBS->CreateEventEx(
        EVT_NOTIFY_SIGNAL,                                      // Type
        TPL_NOTIFY,                                             // NotifyTpl
        ReadyToLock,                                            // NotifyFunction
        NULL,                                                   // NotifyContext
        NULL,                        // EventGroup
        &(gDemo1_Access_Key_Protocol.Demo1_Ready_To_Lock_Event) // Event
    );
    Status = gBS->SetTimer (
        Demo1_Bob_PeriodicTimer,        // Event
        TimerPeriodic,                  // Type
        EFI_TIMER_PERIOD_SECONDS(5));  
    gBS->CloseEvent (Event);
'''

matches = re.findall(gBS_pattern, content)
for match in matches:
    print(match.strip())
