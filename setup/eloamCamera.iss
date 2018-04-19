; -- Example2.iss --
; Same as Example1.iss, but creates its icon in the Programs folder of the
; Start Menu instead of in a subfolder, and also creates a desktop icon.

; SEE THE DOCUMENTATION FOR DETAILS ON CREATING .ISS SCRIPT FILES!

[Setup]
AppName=���֤��Ϣ��ȡ��� V1.0
AppVersion=V1.0
DefaultDirName={pf}\���֤��Ϣ��ȡ��� V1.0
DefaultGroupName=���֤��Ϣ��ȡ��� V1.0
DisableProgramGroupPage=yes
UninstallDisplayIcon={app}\���֤��Ϣ��ȡ��� V1.0
OutputBaseFilename=���֤��Ϣ��ȡ��� V1.0
OutputDir=.\output\����

SetupIconFile=eloam.ico
UninstallIconFile=uninstall.ico
Compression=lzma
SolidCompression=yes
ChangesEnvironment=true

UsePreviousGroup = no
UsePreviousAppDir = no

[Languages]
Name: "cn"; MessagesFile: "compiler:Languages/Chinese.isl" 

[Files]
Source: "..\Win32\Release\IdCard.exe"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\ed.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\el.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\eloamDll.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\video.flt"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\edcom.dll"; DestDir: "{app}"; Flags: replacesameversion

;����֤��ؿ�
Source: "..\Win32\Release\elr.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\sdtapi.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\trendcote.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\WltRS.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\IdCard.exe"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\idcard_trend.dll"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\plugin.ini"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\����֤����.exe"; DestDir: "{app}"; Flags: replacesameversion

Source: "..\Win32\Release\msvcp100.dll"; DestDir: "{app}"
Source: "..\Win32\Release\msvcr100.dll"; DestDir: "{app}"
Source: "..\Win32\Release\mfc100.dll"; DestDir: "{app}"

Source: "..\Win32\Release\Qt5Core.dll"; DestDir: "{app}"
Source: "..\Win32\Release\Qt5Gui.dll"; DestDir: "{app}"
Source: "..\Win32\Release\Qt5Widgets.dll"; DestDir: "{app}"
Source: "..\Win32\Release\icudt51.dll"; DestDir: "{app}"
Source: "..\Win32\Release\icuin51.dll"; DestDir: "{app}"
Source: "..\Win32\Release\icuuc51.dll"; DestDir: "{app}"
Source: "..\Win32\Release\Qt5Xml.dll"; DestDir: "{app}"
                    
Source: "..\Win32\Release\platforms\*"; DestDir: "{app}\platforms"

;ͼ��
Source: "..\Win32\Release\uninstall.ico"; DestDir: "{app}"; Flags: replacesameversion
Source: "..\Win32\Release\eloam.ico"; DestDir: "{app}"; Flags: replacesameversion

[Icons]
Name: "{group}\���֤��Ϣ��ȡ��� V1.0"; Filename: "{app}\IdCard.exe"; IconFilename: "{app}\eloam.ico";
Name: "{group}\{cm:UninstallProgram, ���֤��Ϣ��ȡ��� V1.0}"; Filename: "{uninstallexe}"; IconFilename: "{app}\uninstall.ico";
Name: "{commondesktop}\���֤��Ϣ��ȡ��� V1.0"; Filename: "{app}\IdCard.exe"; IconFilename: "{app}\eloam.ico";

[Run] 
Filename: "{app}\IdCard.exe"; Description: "{cm:LaunchProgram,���֤��Ϣ��ȡ��� V1.0}" ;Flags: postinstall nowait skipifsilent

