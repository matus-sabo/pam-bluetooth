#include <stdio.h>
#include <stdlib.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <string.h>

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
	const char *const macAddresses[] = {"00:00:00:00:00:00"};
	const char *username = "username";
	const char *password = "password";
	int arrLength = sizeof macAddresses / sizeof macAddresses[0];
	char cmd[100] = "";
	const char *pam_username = NULL;
	const char *pam_password = NULL;
	pam_get_item(pamh, PAM_USER, (const void **)&pam_username);
	pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&pam_password, NULL);
	if (strcmp(pam_username, username) == 0 && strlen(pam_password) == 0)
	{
		for (int i = 0; i < arrLength; i++)
		{
			snprintf(cmd, sizeof(cmd), "timeout 5 bluetoothctl connect %s > /dev/null 2>&1", macAddresses[i]);
			system(cmd);
			snprintf(cmd, sizeof(cmd), "timeout 5 bluetoothctl info %s | grep 'Connected: yes' > /dev/null 2>&1", macAddresses[i]);
			int x = system(cmd);
			if (x == 0)
			{
				snprintf(cmd, sizeof(cmd), "timeout 5 bluetoothctl disconnect %s > /dev/null 2>&1 &", macAddresses[i]);
				system(cmd);
				pam_set_item(pamh, PAM_AUTHTOK, password);
				break;
			}
		}
	}
	return PAM_IGNORE;
}
