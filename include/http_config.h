/* ====================================================================
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Apache" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation.  For more
 * information on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 *
 * Portions of this software are based upon public domain software
 * originally written at the National Center for Supercomputing Applications,
 * University of Illinois, Urbana-Champaign.
 */

#ifndef APACHE_HTTP_CONFIG_H
#define APACHE_HTTP_CONFIG_H

#include "ap_hooks.h"
#include "util_cfgtree.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * The central data structures around here...
 */

/* Command dispatch structures... */

/* Note that for all of these except RAW_ARGS, the config routine is
 * passed a freshly allocated string which can be modified or stored
 * or whatever... it's only necessary to do pstrdup() stuff with
 * RAW_ARGS.
 */
enum cmd_how {
    RAW_ARGS,			/* cmd_func parses command line itself */
    TAKE1,			/* one argument only */
    TAKE2,			/* two arguments only */
    ITERATE,			/* one argument, occuring multiple times
				 * (e.g., IndexIgnore)
				 */
    ITERATE2,			/* two arguments, 2nd occurs multiple times
				 * (e.g., AddIcon)
				 */
    FLAG,			/* One of 'On' or 'Off' */
    NO_ARGS,			/* No args at all, e.g. </Directory> */
    TAKE12,			/* one or two arguments */
    TAKE3,			/* three arguments only */
    TAKE23,			/* two or three arguments */
    TAKE123,			/* one, two or three arguments */
    TAKE13			/* one or three arguments */
};

typedef struct cmd_parms_struct cmd_parms;

#ifdef AP_DEBUG

typedef union {
    const char *(*no_args) (cmd_parms *parms, void *mconfig);
    const char *(*raw_args) (cmd_parms *parms, void *mconfig,
			     const char *args);
    const char *(*take1) (cmd_parms *parms, void *mconfig, const char *w);
    const char *(*take2) (cmd_parms *parms, void *mconfig, const char *w,
			  const char *w2);
    const char *(*take3) (cmd_parms *parms, void *mconfig, const char *w,
			  const char *w2, const char *w3);
    const char *(*flag) (cmd_parms *parms, void *mconfig, int on);
} cmd_func;

# define AP_NO_ARGS	func.no_args
# define AP_RAW_ARGS	func.raw_args
# define AP_TAKE1	func.take1
# define AP_TAKE2	func.take2
# define AP_TAKE3	func.take3
# define AP_FLAG	func.flag

# define AP_INIT_NO_ARGS(directive, func, mconfig, where, help) \
    { directive, { .no_args=func }, mconfig, where, RAW_ARGS, help }
# define AP_INIT_RAW_ARGS(directive, func, mconfig, where, help) \
    { directive, { .raw_args=func }, mconfig, where, RAW_ARGS, help }
# define AP_INIT_TAKE1(directive, func, mconfig, where, help) \
    { directive, { .take1=func }, mconfig, where, TAKE1, help }
# define AP_INIT_ITERATE(directive, func, mconfig, where, help) \
    { directive, { .take1=func }, mconfig, where, ITERATE, help }
# define AP_INIT_TAKE2(directive, func, mconfig, where, help) \
    { directive, { .take2=func }, mconfig, where, TAKE2, help }
# define AP_INIT_TAKE12(directive, func, mconfig, where, help) \
    { directive, { .take2=func }, mconfig, where, TAKE12, help }
# define AP_INIT_FLAG(directive, func, mconfig, where, help) \
    { directive, { .flag=func }, mconfig, where, FLAG, help }

#else

typedef const char *(*cmd_func) ();

# define AP_NO_ARGS  func
# define AP_RAW_ARGS func
# define AP_TAKE1    func
# define AP_TAKE2    func
# define AP_TAKE3    func
# define AP_FLAG     func

# define AP_INIT_NO_ARGS(directive, func, mconfig, where, help) \
    { directive, func, mconfig, where, RAW_ARGS, help }
# define AP_INIT_RAW_ARGS(directive, func, mconfig, where, help) \
    { directive, func, mconfig, where, RAW_ARGS, help }
# define AP_INIT_TAKE1(directive, func, mconfig, where, help) \
    { directive, func, mconfig, where, TAKE1, help }
# define AP_INIT_ITERATE(directive, func, mconfig, where, help) \
    { directive, func, mconfig, where, ITERATE, help }
# define AP_INIT_TAKE2(directive, func, mconfig, where, help) \
    { directive, func, mconfig, where, TAKE2, help }
# define AP_INIT_TAKE12(directive, func, mconfig, where, help) \
    { directive, func, mconfig, where, TAKE12, help }
# define AP_INIT_FLAG(directive, func, mconfig, where, help) \
    { directive, func, mconfig, where, FLAG, help }

#endif

typedef struct command_struct {
    const char *name;		/* Name of this command */
    cmd_func func;
    void *cmd_data;		/* Extra data, for functions which
				 * implement multiple commands...
				 */
    int req_override;		/* What overrides need to be allowed to
				 * enable this command.
				 */
    enum cmd_how args_how;	/* What the command expects as arguments */

    const char *errmsg;		/* 'usage' message, in case of syntax errors */
} command_rec;

/* The allowed locations for a configuration directive are the union of
 * those indicated by each set bit in the req_override mask.
 *
 * (req_override & RSRC_CONF)   => *.conf outside <Directory> or <Location>
 * (req_override & ACCESS_CONF) => *.conf inside <Directory> or <Location>
 * (req_override & OR_AUTHCFG)  => *.conf inside <Directory> or <Location>
 *                                 and .htaccess when AllowOverride AuthConfig
 * (req_override & OR_LIMIT)    => *.conf inside <Directory> or <Location>
 *                                 and .htaccess when AllowOverride Limit
 * (req_override & OR_OPTIONS)  => *.conf anywhere
 *                                 and .htaccess when AllowOverride Options
 * (req_override & OR_FILEINFO) => *.conf anywhere
 *                                 and .htaccess when AllowOverride FileInfo
 * (req_override & OR_INDEXES)  => *.conf anywhere
 *                                 and .htaccess when AllowOverride Indexes
 */
#define OR_NONE 0
#define OR_LIMIT 1
#define OR_OPTIONS 2
#define OR_FILEINFO 4
#define OR_AUTHCFG 8
#define OR_INDEXES 16
#define OR_UNSET 32
#define ACCESS_CONF 64
#define RSRC_CONF 128
#define EXEC_ON_READ 256
#define OR_ALL (OR_LIMIT|OR_OPTIONS|OR_FILEINFO|OR_AUTHCFG|OR_INDEXES)

/* This can be returned by a function if they don't wish to handle
 * a command. Make it something not likely someone will actually use
 * as an error code.
 */

#define DECLINE_CMD "\a\b"

/* Common structure for reading of config files / passwd files etc. */
typedef struct {
    int (*getch) (void *param);	/* a getc()-like function */
    void *(*getstr) (void *buf, size_t bufsiz, void *param); /* a fgets()-like function */
    int (*close) (void *param);	/* a close hander function */
    void *param;		/* the argument passed to getch/getstr/close */
    const char *name;		/* the filename / description */
    unsigned line_number;	/* current line number, starting at 1 */
} configfile_t;

/*
 * This structure is passed to a command which is being invoked,
 * to carry a large variety of miscellaneous data which is all of
 * use to *somebody*...
 */

struct cmd_parms_struct
    {
    void *info;			/* Argument to command from cmd_table */
    int override;		/* Which allow-override bits are set */
    int limited;		/* Which methods are <Limit>ed */

    configfile_t *config_file;  /* Config file structure. */
    ap_directive_t *directive;	/* the directive specifying this command */

    ap_pool_t *pool;			/* Pool to allocate new storage in */
    ap_pool_t *temp_pool;		/* Pool for scratch memory; persists during
				 * configuration, but wiped before the first
				 * request is served...
				 */
    server_rec *server;		/* Server_rec being configured for */
    char *path;			/* If configuring for a directory,
				 * pathname of that directory.
				 * NOPE!  That's what it meant previous to the
				 * existance of <Files>, <Location> and regex
				 * matching.  Now the only usefulness that can
				 * be derived from this field is whether a command
				 * is being called in a server context (path == NULL)
				 * or being called in a dir context (path != NULL).
				 */
    const command_rec *cmd;	/* configuration command */

    void *context;		/* per_dir_config vector passed 
				 * to handle_command */
    const ap_directive_t *err_directive; /* directive with syntax error */

};

/* This structure records the existence of handlers in a module... */

typedef struct {
    const char *content_type;	/* MUST be all lower case */
    int (*handler) (request_rec *);
} handler_rec;

/*
 * Module structures.  Just about everything is dispatched through
 * these, directly or indirectly (through the command and handler
 * tables).
 */

typedef struct module_struct {
    int version;		/* API version, *not* module version;
				 * check that module is compatible with this
				 * version of the server.
				 */
    int minor_version;          /* API minor version. Provides API feature
                                 * milestones. Not checked during module init
				 */
    int module_index;		/* Index to this modules structures in
				 * config vectors.
				 */

    const char *name;
    void *dynamic_load_handle;

    struct module_struct *next;

    unsigned long magic;        /* Magic Cookie to identify a module structure;
                                 * It's mainly important for the DSO facility
                                 * (see also mod_so).
                                 */
    void (*rewrite_args) (process_rec *process);
    void *(*create_dir_config) (ap_pool_t *p, char *dir);
    void *(*merge_dir_config) (ap_pool_t *p, void *base_conf, void *new_conf);
    void *(*create_server_config) (ap_pool_t *p, server_rec *s);
    void *(*merge_server_config) (ap_pool_t *p, void *base_conf, void *new_conf);

    const command_rec *cmds;
    const handler_rec *handlers;

    /* Hooks for getting into the middle of server ops...

     * translate_handler --- translate URI to filename
     * access_checker --- check access by host address, etc.   All of these
     *                    run; if all decline, that's still OK.
     * check_user_id --- get and validate user id from the HTTP request
     * auth_checker --- see if the user (from check_user_id) is OK *here*.
     *                  If all of *these* decline, the request is rejected
     *                  (as a SERVER_ERROR, since the module which was
     *                  supposed to handle this was configured wrong).
     * type_checker --- Determine MIME type of the requested entity;
     *                  sets content_type, _encoding and _language fields.
     */

    void (*register_hooks) (void);
} module;

/* Initializer for the first few module slots, which are only
 * really set up once we start running.  Note that the first two slots
 * provide a version check; this should allow us to deal with changes to
 * the API. The major number should reflect changes to the API handler table
 * itself or removal of functionality. The minor number should reflect
 * additions of functionality to the existing API. (the server can detect
 * an old-format module, and either handle it back-compatibly, or at least
 * signal an error). See src/include/ap_mmn.h for MMN version history.
 */

#define STANDARD_MODULE_STUFF	this_module_needs_to_be_ported_to_apache_2_0

#define STANDARD20_MODULE_STUFF	MODULE_MAGIC_NUMBER_MAJOR, \
				MODULE_MAGIC_NUMBER_MINOR, \
				-1, \
				__FILE__, \
				NULL, \
				NULL, \
				MODULE_MAGIC_COOKIE, \
                                NULL      /* rewrite args spot */

#define MPM20_MODULE_STUFF	MODULE_MAGIC_NUMBER_MAJOR, \
				MODULE_MAGIC_NUMBER_MINOR, \
				-1, \
				__FILE__, \
				NULL, \
				NULL, \
				MODULE_MAGIC_COOKIE

/* Generic accessors for other modules to get at their own module-specific
 * data
 */

API_EXPORT(void *) ap_get_module_config(void *conf_vector, module *m);
API_EXPORT(void) ap_set_module_config(void *conf_vector, module *m, void *val);

#define ap_get_module_config(v,m)	\
    (((void **)(v))[(m)->module_index])
#define ap_set_module_config(v,m,val)	\
    ((((void **)(v))[(m)->module_index]) = (val))

/* Generic command handling function... */

API_EXPORT_NONSTD(const char *) ap_set_string_slot(cmd_parms *, void *,
						   const char *);
API_EXPORT_NONSTD(const char *) ap_set_string_slot_lower(cmd_parms *, char *, char *);
API_EXPORT_NONSTD(const char *) ap_set_flag_slot(cmd_parms *, char *, int);
API_EXPORT_NONSTD(const char *) ap_set_file_slot(cmd_parms *, char *, char *);

/* For modules which need to read config files, open logs, etc. ...
 * this returns the fname argument if it begins with '/'; otherwise
 * it relativizes it wrt server_root.
 */

API_EXPORT(const char *) ap_server_root_relative(ap_pool_t *p, const char *fname);

/* Finally, the hook for dynamically loading modules in... */

API_EXPORT(void) ap_add_module(module *m);
API_EXPORT(void) ap_remove_module(module *m);
API_EXPORT(void) ap_add_loaded_module(module *mod);
API_EXPORT(void) ap_remove_loaded_module(module *mod);
API_EXPORT(int) ap_add_named_module(const char *name);
API_EXPORT(void) ap_clear_module_list(void);
API_EXPORT(const char *) ap_find_module_name(module *m);
API_EXPORT(module *) ap_find_linked_module(const char *name);

/* Open a configfile_t as FILE, return open configfile_t struct pointer */
API_EXPORT(ap_status_t) ap_pcfg_openfile(configfile_t **, ap_pool_t *p, const char *name);

/* Allocate a configfile_t handle with user defined functions and params */
API_EXPORT(configfile_t *) ap_pcfg_open_custom(ap_pool_t *p, const char *descr,
    void *param,
    int(*getc_func)(void*),
    void *(*gets_func) (void *buf, size_t bufsiz, void *param),
    int(*close_func)(void *param));

/* Read one line from open configfile_t, strip LF, increase line number */
API_EXPORT(int) ap_cfg_getline(char *buf, size_t bufsize, configfile_t *cfp);

/* Read one char from open configfile_t, increase line number upon LF */
API_EXPORT(int) ap_cfg_getc(configfile_t *cfp);

/* Detach from open configfile_t, calling the close handler */
API_EXPORT(int) ap_cfg_closefile(configfile_t *cfp);

/* for implementing subconfigs and customized config files */
API_EXPORT(const char *) ap_soak_end_container(cmd_parms *cmd, char *directive);
const char * ap_build_cont_config(ap_pool_t *p, ap_pool_t *temp_pool,
                                        cmd_parms *parms,
                                        ap_directive_t **current,
                                        ap_directive_t **curr_parent,
                                        char *orig_directive);
API_EXPORT(const char *) ap_build_config(cmd_parms *parms,
					 ap_pool_t *conf_pool,
					 ap_pool_t *temp_pool,
					 ap_directive_t **conftree);
API_EXPORT(const char *) ap_walk_config(ap_directive_t *conftree,
					cmd_parms *parms, void *config);

/* ap_check_cmd_context() definitions: */
API_EXPORT(const char *) ap_check_cmd_context(cmd_parms *cmd, unsigned forbidden);

/* ap_check_cmd_context():              Forbidden in: */
#define  NOT_IN_VIRTUALHOST     0x01 /* <Virtualhost> */
#define  NOT_IN_LIMIT           0x02 /* <Limit> */
#define  NOT_IN_DIRECTORY       0x04 /* <Directory> */
#define  NOT_IN_LOCATION        0x08 /* <Location> */
#define  NOT_IN_FILES           0x10 /* <Files> */
#define  NOT_IN_DIR_LOC_FILE    (NOT_IN_DIRECTORY|NOT_IN_LOCATION|NOT_IN_FILES) /* <Directory>/<Location>/<Files>*/
#define  GLOBAL_ONLY            (NOT_IN_VIRTUALHOST|NOT_IN_LIMIT|NOT_IN_DIR_LOC_FILE)


#ifdef CORE_PRIVATE

extern API_VAR_EXPORT module *top_module;

extern API_VAR_EXPORT module *ap_prelinked_modules[];
extern API_VAR_EXPORT module *ap_preloaded_modules[];
extern API_VAR_EXPORT module **ap_loaded_modules;

/* For mod_so.c... */

void ap_single_module_configure(ap_pool_t *p, server_rec *s, module *m);

/* For http_main.c... */

API_EXPORT(void) ap_setup_prelinked_modules(process_rec *process);
API_EXPORT(void) ap_show_directives(void);
API_EXPORT(void) ap_show_modules(void);
API_EXPORT(server_rec*) ap_read_config(process_rec *process, ap_pool_t *temp_pool, const char *config_name, ap_directive_t **conftree);
API_EXPORT(void) ap_pre_config_hook(ap_pool_t *pconf, ap_pool_t *plog, ap_pool_t *ptemp, server_rec *s);
API_EXPORT(void) ap_post_config_hook(ap_pool_t *pconf, ap_pool_t *plog, ap_pool_t *ptemp, server_rec *s);
API_EXPORT(void) ap_run_rewrite_args(process_rec *process);
API_EXPORT(void) ap_register_hooks(module *m);
API_EXPORT(void) ap_fixup_virtual_hosts(ap_pool_t *p, server_rec *main_server);

/* For http_request.c... */

void *ap_create_request_config(ap_pool_t *p);
CORE_EXPORT(void *) ap_create_per_dir_config(ap_pool_t *p);
void *ap_merge_per_dir_configs(ap_pool_t *p, void *base, void *new);

/* For http_connection.c... */

void *ap_create_conn_config(ap_pool_t *p);

/* For http_core.c... (<Directory> command and virtual hosts) */

int ap_parse_htaccess(void **result, request_rec *r, int override,
		const char *path, const char *access_name);

CORE_EXPORT(const char *) ap_init_virtual_host(ap_pool_t *p, const char *hostname,
				server_rec *main_server, server_rec **);
void ap_process_resource_config(server_rec *s, const char *fname, 
                 ap_directive_t **conftree, ap_pool_t *p, ap_pool_t *ptemp);
API_EXPORT(void) ap_process_config_tree(server_rec *s, ap_directive_t *conftree,
                                        ap_pool_t *p, ap_pool_t *ptemp);


/* For individual MPMs... */

void ap_child_init_hook(ap_pool_t *pchild, server_rec *s);

/* Module-method dispatchers, also for http_request.c */

int ap_translate_name(request_rec *);
int ap_check_user_id(request_rec *);	/* obtain valid username from client auth */
int ap_invoke_handler(request_rec *);

/* for mod_perl */

CORE_EXPORT(const command_rec *) ap_find_command(const char *name, const command_rec *cmds);
CORE_EXPORT(const command_rec *) ap_find_command_in_modules(const char *cmd_name, module **mod);
CORE_EXPORT(void *) ap_set_config_vectors(cmd_parms *parms, void *config, module *mod);
CORE_EXPORT(const char *) ap_handle_command(cmd_parms *parms, void *config, const char *l);

#endif

  /* Hooks */
AP_DECLARE_HOOK(int,header_parser,(request_rec *))
AP_DECLARE_HOOK(void,pre_config,
	     (ap_pool_t *pconf,ap_pool_t *plog,ap_pool_t *ptemp))
AP_DECLARE_HOOK(void,post_config,
	     (ap_pool_t *pconf,ap_pool_t *plog,ap_pool_t *ptemp,server_rec *s))
AP_DECLARE_HOOK(void,open_logs,
	     (ap_pool_t *pconf,ap_pool_t *plog,ap_pool_t *ptemp,server_rec *s))
AP_DECLARE_HOOK(void,child_init,(ap_pool_t *pchild, server_rec *s))

#ifdef __cplusplus
}
#endif

#endif	/* !APACHE_HTTP_CONFIG_H */
