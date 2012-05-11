extern void TestFtspC$Boot$booted(void);
extern void /*TimeSyncC.TimeSyncP*/TimeSyncP$0$Boot$booted(void);


void BootFunction()
{
	TimeSyncP$0$Boot$booted();
    TestFtspC$Boot$booted();
}