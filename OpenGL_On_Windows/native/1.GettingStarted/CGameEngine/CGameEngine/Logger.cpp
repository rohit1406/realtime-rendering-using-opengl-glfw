#include"Logger.h"

void openFileLogger()
{
	if (fopen_s(&gLogfile, "ppLog.txt", "w") != 0)
	{
		MessageBox(NULL, TEXT("Log File Can Not Be Created\nExitting..."), TEXT("Error"), MB_OK | MB_TOPMOST | MB_ICONSTOP);
		//uninitialize();
	}
	else
	{
		fprintf(gLogfile, "\nLog file is opened successfully...\n");
	}
}

void closeFileLogger()
{
	if (gLogfile) {
		fprintf(gLogfile, "\nLog file is closed successfully...\n");
		fclose(gLogfile);
		gLogfile = NULL;
	}
}

void logStaticData(const char data[])
{
	if (gLogfile)
	{
		fprintf(gLogfile, "RSM: ");
		fprintf(gLogfile, data);
		fprintf(gLogfile, "\n");
	}
}