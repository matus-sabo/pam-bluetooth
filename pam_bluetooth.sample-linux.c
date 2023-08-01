#include <stdio.h>
#include <stdlib.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <string.h>

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
	const char *const mac_addresses[] = {"00:00:00:00:00:00"};
	const char *username = "username";
	const char *password = "password";
	int mac_addresses_length = sizeof mac_addresses / sizeof mac_addresses[0];
	char cmd[100] = "";
	int cmd_exit_code = 0;
	const char *pam_username = NULL;
	const char *pam_password = NULL;
	pam_get_item(pamh, PAM_USER, (const void **)&pam_username);
	pam_get_authtok(pamh, PAM_AUTHTOK, (const char **)&pam_password, NULL);
	if (strcmp(pam_username, username) != 0 || strlen(pam_password) != 0)
	{
		return PAM_IGNORE;
	}
	for (int i = 0; i < mac_addresses_length; i++)
	{
		snprintf(cmd, sizeof(cmd), "timeout 1 bluetoothctl devices | grep %s > /dev/null 2>&1", mac_addresses[i]);
		cmd_exit_code = system(cmd);
		if (cmd_exit_code != 0)
		{
			continue;
		}
		snprintf(cmd, sizeof(cmd), "timeout 5 bluetoothctl connect %s > /dev/null 2>&1", mac_addresses[i]);
		system(cmd);
		snprintf(cmd, sizeof(cmd), "timeout 1 bluetoothctl info %s | grep 'Connected: yes' > /dev/null 2>&1", mac_addresses[i]);
		cmd_exit_code = system(cmd);
		if (cmd_exit_code == 0)
		{
			snprintf(cmd, sizeof(cmd), "timeout 5 bluetoothctl disconnect %s > /dev/null 2>&1 &", mac_addresses[i]);
			system(cmd);
		}
		if (cmd_exit_code != 0)
		{
			continue;
		}
		pam_set_item(pamh, PAM_AUTHTOK, password);
		break;
	}
	return PAM_IGNORE;
}
