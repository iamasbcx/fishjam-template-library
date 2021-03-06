#ifndef FDRIVER_KERNEL_H
#define FDRIVER_KERNEL_H

//NtCurrentPeb()->KernelCallbackTable

/******************************************************************************************************************
*   
*   
******************************************************************************************************************/

/******************************************************************************************************************
* 系统进程分析
*   csrss.exe -- Windows子系统服务器进程, 会保存所有进程的句柄，桌面的窗体句柄即属于这个进程。
*     Console窗体的 ScrollDC 等API需要Hook这个进程才行？
*     win32!RawInputThread 通过 GUID_CLASS_KEYBOARD 获得键盘设备栈中PDO的符号连接名
*   Idle -- 系统的特殊进程，没有对应的 .exe 文件，里面会执行 IO定时器等
*   System -- 系统的特殊进程
*     当驱动加载的时候，系统进程启动新的线程，调用对象管理器，创建一个驱动对象(DRIVER_OBJECT)，然后调用其 DriverEntry 
*   WinLogon.exe
******************************************************************************************************************/

/******************************************************************************************************************
* Ntdll.dll --
*   KiFastSystemCall -- 
******************************************************************************************************************/

/******************************************************************************************************************
* Win32k.sys -- Windows子系统在核心侧的实现，主要负责 User(窗体管理)、GDI、DX(dxg.sys)的入口 -- Dxthunksto dxg.sys
*   里面有三个表(table)?
*     1.W32pServiceTable(function & Return value)
*     2.W32pArgument
*     3.TableProvidedto NT kernel via KeAddSystemServiceTableon initialization
******************************************************************************************************************/

#endif //FDRIVER_KERNEL_H