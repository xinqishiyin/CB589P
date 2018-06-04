

void eventHandler(void)
{
	switch(mFlag.SysMode)
	{
		case SYS_MODE_LINE:
		{
			initSysModeLine();
			sysModeLine();
			break;
		}
		case SYS_MODE_WIRELESS:
		{
			initSysModeWireless();
			sysModeWireless();
			break;
		}
		case SYS_MODE_INTERPHONE:
		{
			sysModeInterphone();
			break;
		}
		default: break;
	}
}