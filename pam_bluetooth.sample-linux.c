#include <stdio.h>
#include <stdlib.h>
#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

size_t GET_COMMAND_OUTPUT_SIZE = 256;

struct options
{
	char controller_mac_address[32];
	char allow_mac_address[32];
	char user[512];
	char encrypted_authtok[1024];
	bool valid;
};

int get_command_output(char *command, char *output)
{

	FILE *file = popen(command, "r");
	if (file)
	{
		char *buffer;
		size_t buffer_size = 0;
		size_t chars = getline(&buffer, &buffer_size, file);
		if (strlen(buffer) > 0 && (buffer)[chars - 1] == '\n')
		{
			(buffer)[chars - 1] = '\0';
		}
		strncpy(output, buffer, GET_COMMAND_OUTPUT_SIZE);
		output[GET_COMMAND_OUTPUT_SIZE - 1] = '\0';
	}
	pclose(file);
}

void parse_options(int argc, const char **argv, struct options *opts)
{
	char *token;
	opts->valid = false;

	for (int i = 0; i < argc; i++)
	{
		if (!strncmp(argv[i], "allow=", 6))
		{
			char *copy = strdup((char *)argv[i] + 6);
			token = strtok(copy, "/");
			if (token != NULL)
			{
				strncpy(opts->controller_mac_address, token, sizeof(opts->controller_mac_address));
				opts->controller_mac_address[sizeof(opts->controller_mac_address) - 1] = '\0';
			}
			token = strtok(NULL, "/");
			if (token != NULL)
			{
				strncpy(opts->allow_mac_address, token, sizeof(opts->allow_mac_address));
				opts->allow_mac_address[sizeof(opts->allow_mac_address) - 1] = '\0';
			}
			free(copy);
		}
		if (!strncmp(argv[i], "user=", 5))
		{
			strncpy(opts->user, (char *)argv[i] + 5, sizeof(opts->user));
			opts->user[sizeof(opts->user) - 1] = '\0';
		}
		if (!strncmp(argv[i], "encrypted_authtok=", 18))
		{
			strncpy(opts->encrypted_authtok, (char *)argv[i] + 18, sizeof(opts->encrypted_authtok));
			opts->encrypted_authtok[sizeof(opts->encrypted_authtok) - 1] = '\0';
		}
	}
	if (strlen(opts->controller_mac_address) > 0 && strlen(opts->allow_mac_address) > 0 && strlen(opts->user) > 0 && strlen(opts->encrypted_authtok) > 0)
	{
		opts->valid = true;
	}
}

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
	struct options opts;
	parse_options(argc, argv, &opts);
	if (!opts.valid)
	{
		return PAM_IGNORE;
	}
	char cmd[2048] = "";
	int cmd_exit_code = 0;
	const char *pam_username = NULL;
	const char *pam_password = NULL;
	pam_get_user(pamh, &pam_username, NULL);
	pam_get_authtok(pamh, PAM_AUTHTOK, &pam_password, NULL);
	if (strcmp(pam_username, opts.user) != 0 || strlen(pam_password) != 0)
	{
		return PAM_IGNORE;
	}
	snprintf(cmd, sizeof(cmd), "timeout 1 bluetoothctl devices | grep %s > /dev/null 2>&1", opts.allow_mac_address);
	cmd_exit_code = system(cmd);
	if (cmd_exit_code != 0)
	{
		return PAM_IGNORE;
	}
	snprintf(cmd, sizeof(cmd), "timeout 5 bluetoothctl connect %s > /dev/null 2>&1", opts.allow_mac_address);
	system(cmd);
	snprintf(cmd, sizeof(cmd), "timeout 1 bluetoothctl info %s | grep 'Connected: yes' > /dev/null 2>&1", opts.allow_mac_address);
	cmd_exit_code = system(cmd);
	if (cmd_exit_code != 0)
	{
		return PAM_IGNORE;
	}
	snprintf(cmd, sizeof(cmd), "timeout 5 bluetoothctl disconnect %s > /dev/null 2>&1 &", opts.allow_mac_address);
	system(cmd);
	char link_key[GET_COMMAND_OUTPUT_SIZE];
	char password[GET_COMMAND_OUTPUT_SIZE];
	snprintf(cmd, sizeof(cmd), "runcon -u unconfined_u -r unconfined_r -t unconfined_t cat /var/lib/bluetooth/%s/%s/info | grep -A1 '\\[LinkKey\\]' | tail -n 1", opts.controller_mac_address, opts.allow_mac_address);
	get_command_output(cmd, link_key);
	snprintf(cmd, sizeof(cmd), "echo %s | openssl enc -base64 -d -aes-256-cbc -salt -pass pass:%s -pbkdf2", opts.encrypted_authtok, link_key);
	get_command_output(cmd, password);
	pam_set_item(pamh, PAM_AUTHTOK, &password);
	return PAM_IGNORE;
}
