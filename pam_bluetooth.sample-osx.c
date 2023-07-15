#include <stdio.h>
#include <stdlib.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	return PAM_IGNORE;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	return PAM_IGNORE;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	const char *const macAddresses[] = {"00:00:00:00:00:00"};
	int arrLength = sizeof macAddresses / sizeof macAddresses[0];
	char cmd[100] = "";
	for (int i = 0; i < arrLength; i++)
	{
		snprintf(cmd, sizeof(cmd), "timeout 5 blueutil --connect %s > /dev/null 2>&1", macAddresses[i]);
		system(cmd);
		snprintf(cmd, sizeof(cmd), "blueutil --info %s | grep -q ', connected'", macAddresses[i]);
		int x = system(cmd);
		if (x == 0)
		{
			pam_set_item(pamh, PAM_AUTHTOK, "password");
			return PAM_SUCCESS;
		}
	}
	return PAM_IGNORE;
}