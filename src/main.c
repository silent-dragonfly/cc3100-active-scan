#define __MAIN_C__
#include "main.h"
#include "windows.h"

#define SCAN_TABLE_SIZE  20
#define SCAN_INTERVAL    10

int main(int argc, char** argv) {
	Sl_WlanNetworkEntry_t netEntries[SCAN_TABLE_SIZE];
	memset(netEntries, 0, sizeof(netEntries));

	_i16 retVal = sl_Start(0, 0, 0);
	if ((retVal < 0) || (ROLE_STA != retVal)) {
		DEBUG(" Failed to start the device");
		return -1;
	}

	DEBUG("Device started as STATION");

	/* enable scan, and set scan interval */
	_u8 policyOpt = SL_SCAN_POLICY(1);
	_u32 policyVal = SCAN_INTERVAL;

	retVal = sl_WlanPolicySet(SL_POLICY_SCAN, policyOpt, (_u8 *) &policyVal,
			sizeof(policyVal));
	if (retVal < 0) {
		DEBUG(" Failed to Enable the scan policy \n\r");
		return -1;
	}

	/* delay 1 second to verify scan is started */
	Sleep(1000);

	/* get scan results - all 20 entries in one transaction */
	_u16 runningIdx = 0;
	_u32 numOfEntries = SCAN_TABLE_SIZE;

	/* retVal indicates the valid number of entries */
	/* The scan results are occupied in netEntries[] */
	retVal = sl_WlanGetNetworkList(runningIdx, numOfEntries,
			&netEntries[runningIdx]);

	printf("      BSSID      \tRSSI\tSSID\n");
	for (int i = 0; i < retVal; i++) {
		printf("%02X:%02X:%02X:%02X:%02X:%02X\t%d\t%s\n",
				netEntries[i].bssid[0], netEntries[i].bssid[1],
				netEntries[i].bssid[2], netEntries[i].bssid[3],
				netEntries[i].bssid[4], netEntries[i].bssid[5],
				netEntries[i].rssi, netEntries[i].ssid);
	}
	DEBUG("Scan Process completed");

	/* Stop the CC3100 device */
	retVal = sl_Stop(SL_STOP_TIMEOUT);
	if (retVal < 0) {
		DEBUG("Can not stop device gracefully");
		return -1;
	}

	return 0;
}
