#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

#define OUT_BUFSIZE 256

static char *ngx_htt_hello_set(ngx_conf_t *cf,ngx_command_t *cmd,void *conf);
static char *ngx_http_foo_Set(ngx_conf_t *cf,ngx_command_t *cmd,void *conf);

static ngx_int_t ngx_http_hello_process_init(ngx_cycle_t *cycle);
static void ngx_http_hello_proces_exit(ngx_cycle_t *cycle);

static  ngx_int_t make_http_header(ngx_http_request *r);
static ngx_int_t make_http_get_body(ngx_http_request *r,char *out_buf);

static char g_foo_settings[64]={0};

/*Commands*/

static ngx_command_t ngx_http_hello_commands[]={
	{
		ngx_string("ngx_hello_module"),
		NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
		ngx_http_hello_set,
		NGX_HTTP_LOC_CONF_OFFSET,
		0,
		NULL
	},
	{
		ngx_string("hello"),
		NGX_HTTP_LOC_CONF|NGX_CONF_TAKE1,
		ngx_hello_foo_set,
		NGX_HTTP_LOC_CONF_OFFSET,
		0,
		NULL
	},
	ngx_null_command
};

static ngx_http_module_t ngx_http_hello_module_ctx={
	NULL,
	NULL,

	NULL,
	NULL,

	NULL,
	NULL,

	NULL,
	NULL
};

/*hook*/

ngx_module_t ngx_http_hello_module={
	NGX_MODULE_V1,
	&ngx_http_hello_module_ctx,
	ngx_http_hello_commands,
	NGX_HTTP_MODULE,
	NULL,
	NULL,
	ngx_http_hello_process_init,
	NULL,
	NULL,
	ngx_http_hello_process_init,
	NULL,
	NGX_MODULE_V1_PADDING
};

/*setting header for no-cache*/

static ngx_int_t make_http_header(ngx_http_request_t *r){
	ngx_uint_t  i;
	ngx_table_elt_t *cc,*ccp;

	r->headers_out.content_type.len=sizeof("text/html")-1;
	r->headers_out.content_type.data=(u_char *)"text/html";

	ccp = r->headers_out.cache_control.elts;

	if(ccp==NULL){
		if(ngx_array_init(&r->headers_out.cache_control,r->pool,1,sizeof(ngx_table_elt_t *))!=NGX_OK){
			return NGX_ERROR;
		}

		ccp = ngx_array_push(&r->headers_out.cache_control);
		if(ccp==NULL){
			return NGX_ERROR;
		}

		cc=ngx_list_push(&r->headers_out.headers);
		if(cc==NULL){
			return NGX_ERROR;
		}
	}
}
