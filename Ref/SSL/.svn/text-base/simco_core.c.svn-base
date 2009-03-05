/*
 * simco_core.c
 *
 * Copyright (C) 2005, Ranch Networks, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 *
 * Author : Srivatsa Chivukula <srivatsa@ranchnetworks.com>
 *
 */

#include "simco_core.h"
#include "simco_client.h"
#include "simco_defs.h"
#include "simco_nb.h"

int c_debug=0;

#ifndef NO_SSL
static int c_showcerts=0;
static BIO *bio_c_out=NULL;
static SSL_CTX *ctx=NULL;

#define CONF LHASH

CONF *config=NULL;
BIO *bio_err=NULL;
#endif

/* static int in_init = 1; */

MCAgentCon conn[MAX_CON];

static char cert[512];
static unsigned int transactionId;
static int init_all = 0;

static pthread_mutex_t myMutex;

extern int highestFd;

#ifdef SIGPIPE
#define do_pipe_sig()signal(SIGPIPE,SIG_IGN)
#else
#define do_pipe_sig()
#endif

#ifndef NO_SSL
#if defined(WIN32)
#    ifdef _O_BINARY
#      define apps_startup() \
_fmode=_O_BINARY; do_pipe_sig(); CRYPTO_malloc_init(); SSLeay_add_all_algorithms()
#    else
#      define apps_startup() \
_fmode=O_BINARY; do_pipe_sig(); CRYPTO_malloc_init(); SSLeay_add_all_algorithms()
#    endif
#  else
#    define apps_startup()do_pipe_sig(); SSLeay_add_all_algorithms();
#endif
#endif

static int recieveFullPacket(char* buff, int len);
#ifdef NO_SSL
static int checkSizeToRead(char *buff, int len);
#endif
extern int set_cert_stuff(SSL_CTX *ctx, char *cert_file, char *key_file);

/**
 * Initializes the SSL library and global variables.
 * Creates the SSL_ctx and sets its properties. This function
 * should be called once per process.
 * @param trust_cert certificate filename (with path) if SSL is used, NULL otherwise.
 * @return 1 on success, 0 on errror
 */
int mc_init(char *trust_cert)
{
#ifndef NO_SSL
  X509_STORE *store = NULL;
  int state=0;
  char *cert_file=NULL, *key_file=NULL;
  char *cipher=NULL;
  int verify=SSL_VERIFY_PEER; /* SSL_VERIFY_NONE */
  int vflags = 0;
  SSL_METHOD *meth=NULL;
  char *inrand=NULL;
#endif

  int i;

  if (!init_all)
    init_all = 1;
  else
    return 1;

#ifndef NO_SSL
  apps_startup();
#endif

  pthread_mutex_init(&myMutex, NULL);

  transactionId = 1;

  for (i = 0; i < MAX_CON; i++)
    conn[i].sock = -1;

  if (!nb_init())
    return 0;

#ifndef NO_SSL
  c_showcerts=0;

  verify_depth=0;
  verify_error=X509_V_OK;

  OpenSSL_add_ssl_algorithms();
  SSL_load_error_strings();

  meth = SSLv3_client_method();

  if (!app_RAND_load_file(NULL, bio_err, 1) && inrand == NULL && !RAND_status())
    BIO_printf(bio_err,"warning, not much extra random data\n");
  if (inrand != NULL)
    BIO_printf(bio_err,"%ld semi-random bytes loaded\n", app_RAND_load_files(inrand));

  if (bio_c_out == NULL)
    {
      if (!c_debug)
        bio_c_out=BIO_new(BIO_s_null());
      else
        bio_c_out=BIO_new_fp(stdout, BIO_NOCLOSE);
    }

  if (ctx == NULL)
    {
      ctx=SSL_CTX_new(meth);
    }

  if (ctx == NULL)
    {
      ERR_print_errors(bio_err);
      goto end;
    }

  SSL_CTX_set_options(ctx, 0);

  if (state)
    SSL_CTX_set_info_callback(ctx, apps_ssl_info_callback);

  if (cipher != NULL)
    if(!SSL_CTX_set_cipher_list(ctx, cipher))
      {
        BIO_printf(bio_err, "error setting cipher list\n");
        ERR_print_errors(bio_err);
        goto end;
      }

  SSL_CTX_set_verify(ctx, verify, verify_callback);
  if (!set_cert_stuff(ctx, cert_file, key_file))
    goto end;

  strcpy(cert, trust_cert);
  fflush(stdout);
  if ((!SSL_CTX_load_verify_locations(ctx,trust_cert,NULL)) ||
      (!SSL_CTX_set_default_verify_paths(ctx)))
    {
      printf("Error setting default verify locations %s\n", trust_cert);
      ERR_print_errors_fp(stdout);
      goto end;
    }

  store = SSL_CTX_get_cert_store(ctx);
  X509_STORE_set_flags(store, vflags);

#endif

  return 1;

 end:
  pthread_mutex_destroy(&myMutex);
  nb_destroy();

#ifndef NO_SSL
  if (ctx != NULL) SSL_CTX_free(ctx);
  if (bio_c_out != NULL)
    {
      BIO_free(bio_c_out);
      bio_c_out=NULL;
    }

  /* changing from 0.9.7d to 0.9.6.m
  apps_shutdown();
  */
#endif

  init_all = 0;
  exit(1);
}

/**
 * Connect to the specified host at the specified port
 * @param host IP address of the middlebox.
 * @param port port on which simco service is running.
 * @return index to the connection structure, or -1 on error.
 */
int midcomAgentConnect(char *host, int port)
{
  int i = 0, s;
  int overflow = 0;
  int inthost = 0;
#ifndef NO_SSL
  BIO *sbio;
#endif

  assert(host);
  assert(port);

  if ((inthost = inet_addr(host)) == INADDR_NONE)
    return -1;

  if (!init_all)
    mc_init(cert);

  if (init_client(&s, host, port) == 0)
    {
      if (c_debug)
#ifndef NO_SSL
        BIO_printf(bio_err,"connect:errno=%d\n",get_last_socket_error());
#else
      perror("connect");
#endif
      /* SHUTDOWN(s); */
      return -1;
    }

  pthread_mutex_lock (&myMutex);

  for (i = 0; i < MAX_CON; i++)
    if (conn[i].sock == -1) break;

  if (i == MAX_CON)
    overflow = 1;
#ifndef NO_SSL
  else
    conn[i].sslcon = SSL_new(ctx);
#endif

  pthread_mutex_unlock (&myMutex);

  if (overflow)
    return -1;
  if (c_debug)
    printf("Connected(%08X)\n", s);

  if (s > highestFd)
    highestFd = s;
  conn[i].sock = s;
  conn[i].sbuf = NULL;
  conn[i].sbuf_offset = 0;

#ifndef NO_SSL
  sbio=BIO_new_socket(s,BIO_NOCLOSE);

  SSL_set_bio(conn[i].sslcon,sbio,sbio);
  SSL_set_connect_state(conn[i].sslcon);
#endif

  if (c_debug)
    printf ("Connected to ... %s\n", host);

  return i;
}

/**
 * Writes to SSL socket.
 * @param con ssl socket connection.
 * @param cbuf write buffer.
 * @param cbuf_len write buffer length.
 * @return 1 on success, <= 0 on error.
 */
int rnMCAgentCommWrite(MCAgentCon *con, char *cbuf, int cbuf_len)
{
  fd_set writefds;
  int ssl_pending;
  int width;
  int i, k;
  int cbuf_off = 0;
  int *buff = (int*)cbuf;

  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  width = con->sock + 1;

  if ((cbuf != NULL) && (cbuf_len != 0))
    {
      FD_ZERO(&writefds);
      FD_SET(con->sock, &writefds);
      if ((i = select(width, NULL, (void *)&writefds, NULL, NULL)) < 0)
#ifndef NO_SSL
	BIO_printf(bio_err, "bad select %d\n", get_last_socket_error());
#else
      perror("select");
#endif
      else
#ifndef NO_SSL
	if (!(ssl_pending = SSL_pending(con->sslcon)) && FD_ISSET(con->sock, &writefds))
#endif
	{
	  buff[1] = htonl(transactionId);
	write_again:
#ifndef NO_SSL
	  k = SSL_write(con->sslcon, &(cbuf[cbuf_off]), (unsigned int)cbuf_len);
	  switch (SSL_get_error(con->sslcon, k))
	    {
	    case SSL_ERROR_NONE:
	      cbuf_off += k;
	      cbuf_len -= k;
	      if (cbuf_len > 0)
		goto write_again;
	      break;
	    case SSL_ERROR_WANT_WRITE:
	      if (c_debug)
		BIO_printf(bio_c_out,"write W BLOCK\n");
	      break;
	    case SSL_ERROR_WANT_READ:
	      if (c_debug)
		BIO_printf(bio_c_out,"write R BLOCK\n");
	      break;
	    case SSL_ERROR_WANT_X509_LOOKUP:
	      if (c_debug)
		BIO_printf(bio_c_out,"write X BLOCK\n");
	      break;
	    case SSL_ERROR_ZERO_RETURN:
	      if (c_debug)
		BIO_printf(bio_c_out,"shutdown\n");
	      return 0;
	    case SSL_ERROR_SYSCALL:
	      if (c_debug)
		BIO_printf(bio_err,"write:errno=%d\n", get_last_socket_error());
	      return 0;
	    case SSL_ERROR_SSL:
	      if (c_debug)
		BIO_printf(bio_c_out, "write: connection closed by peer\n");
	      return 0;
	    }
#else
          k = write(con->sock, &(cbuf[cbuf_off]), (unsigned int)cbuf_len);
	  cbuf_off += k;
	  cbuf_len -= k;
	  if (cbuf_len > 0)
	    goto write_again;
#endif
	  return ++transactionId-1;
	}
    }
  return 0;
}

/**
 * Reads from SSL socket.
 * @param con ssl socket connection.
 * @param sbuf read buffer.
 * @param offset read buffer length.
 * @return 1 on success, <= 0 on error.
 */
int rnMCAgentCommRead(MCAgentCon *con, char *sbuf, int *offset)
{
  int k;

#ifdef NO_SSL
  char *temp_buf;
  int sbuf_max_read = MAX_READ_SIZE;
  int size_to_read = 0;
#endif

#ifndef NO_SSL
  k = SSL_read(con->sslcon, &(sbuf[*offset]), MAX_READ_SIZE );
  switch (SSL_get_error(con->sslcon, k))
    {
    case SSL_ERROR_NONE:
      if (k <= 0)
	return 0;
      *offset += k;
      break;
    case SSL_ERROR_WANT_WRITE:
      if (c_debug)
	BIO_printf(bio_c_out, "read W BLOCK\n");
      break;
    case SSL_ERROR_WANT_READ:
      if (c_debug)
	BIO_printf(bio_c_out, "read R BLOCK\n");
      break;
    case SSL_ERROR_WANT_X509_LOOKUP:
      if (c_debug)
	BIO_printf(bio_c_out, "read X BLOCK\n");
      break;
    case SSL_ERROR_SYSCALL:
      if (c_debug)
	BIO_printf(bio_err, "read:errno=%d\n", get_last_socket_error());
      return 0;
    case SSL_ERROR_ZERO_RETURN:
      if (c_debug)
	BIO_printf(bio_c_out, "closed\n");
      return 0;
    case SSL_ERROR_SSL:
      if (c_debug)
	BIO_printf(bio_c_out, "read: connection closed by peer\n");
      return 0;
    }
#else
  temp_buf = (char*)malloc(sbuf_max_read);
  k = recv(con->sock, temp_buf, sbuf_max_read, MSG_PEEK); // pre-read check
  size_to_read = checkSizeToRead(temp_buf, k);
  if (size_to_read != 0)
    sbuf_max_read = size_to_read;

  k = read(con->sock, &(sbuf[*offset]), sbuf_max_read);
  if (k <= 0)
    return 0;
  *offset += k;
#endif
  if ( ! recieveFullPacket(sbuf, *offset) )
    {
      printf("Failed after reading %d bytes \n", *offset);
      return -1;
    }
  return 1;
}

/**
 * Writes to and reads from SSL socket.
 * @param con ssl socket connection.
 * @param cbuf write buffer.
 * @param cbuf_len write buffer length.
 * @param sbuf read buffer.
 * @param sbuf_len read buffer length.
 * @return 1 on success, <= 0 on error.
 */
int rnMCAgentComm(MCAgentCon *con, char *cbuf, int cbuf_len, char **sbuf, int *sbuf_len)
{
  fd_set readfds,writefds;
  int ssl_pending;
  int width;
  /* int full_log=1; */
  int i, k, step;
  int cbuf_off=0;
  int sbuf_max_read = *sbuf_len;
  int sbuf_remain = *sbuf_len;
  int *buff = (int*)cbuf;
#ifdef NO_SSL
  char *temp_buf;
  int size_to_read;
#endif
  struct timeval tv;

  tv.tv_sec = 1;
  tv.tv_usec = 0;

  width = con->sock + 1;

  /*
  if (c_debug)
    {
      if (SSL_in_init(con->sslcon) && !SSL_total_renegotiations(con->sslcon))
        {
          in_init=1;
        }
      else
        {
          if (in_init)
            {
              in_init=0;
              print_stuff(bio_c_out,con,full_log);
              if (full_log > 0) full_log--;
            }
        }
      BIO_printf(bio_c_out,"---\nSSL handshake has read %ld bytes and written %ld bytes\n",
                 BIO_number_read(SSL_get_rbio(con->sslcon)),
                 BIO_number_written(SSL_get_wbio(con->sslcon)));
    }
  */

  if ((cbuf != NULL) && (cbuf_len != 0))
    {
      FD_ZERO(&writefds);
      FD_SET(con->sock, &writefds);
      i = select(width, NULL, (void *)&writefds, NULL, NULL);
      if (i < 0)
#ifndef NO_SSL
	BIO_printf(bio_err, "bad write select %d\n", get_last_socket_error());
#else
      perror("bad write select");
#endif
      else if (i == 0)
	{
	  printf("write select timed out\n");
	}
      else
#ifndef NO_SSL
	if (!(ssl_pending = SSL_pending(con->sslcon)) && FD_ISSET(con->sock, &writefds))
#endif
	{
	  buff[1] = htonl(transactionId);
	write_again:
#ifndef NO_SSL
	  k = SSL_write(con->sslcon, &(cbuf[cbuf_off]), (unsigned int)cbuf_len);
	  switch (SSL_get_error(con->sslcon, k))
	    {
	    case SSL_ERROR_NONE:
	      cbuf_off += k;
	      cbuf_len -= k;
	      if (cbuf_len > 0)
		goto write_again;
	      break;
	    case SSL_ERROR_WANT_WRITE:
	      if (c_debug)
		BIO_printf(bio_c_out, "write W BLOCK\n");
	      break;
	    case SSL_ERROR_WANT_READ:
	      if (c_debug)
		BIO_printf(bio_c_out, "write R BLOCK\n");
	      break;
	    case SSL_ERROR_WANT_X509_LOOKUP:
	      if (c_debug)
		BIO_printf(bio_c_out, "write X BLOCK\n");
	      break;
	    case SSL_ERROR_ZERO_RETURN:
	      if (c_debug)
		BIO_printf(bio_c_out, "shutdown\n");
	      return 0;
	    case SSL_ERROR_SYSCALL:
	      if (c_debug)
		BIO_printf(bio_c_out, "write:errno=%d\n", get_last_socket_error());
	      return 0;
	    case SSL_ERROR_SSL:
	      if (c_debug)
		BIO_printf(bio_c_out, "write: connection closed by peer\n");
	      return 0;
	    }
#else
          k = write(con->sock, &(cbuf[cbuf_off]), (unsigned int)cbuf_len);
	  cbuf_off += k;
	  cbuf_len -= k;
	  if (cbuf_len > 0)
	    goto write_again;
#endif
	  transactionId++;
	}
    }

  *sbuf_len = 0;
  step = 2;

  for (;;)
    {
      FD_ZERO(&readfds);
      FD_SET(con->sock, &readfds);
      tv.tv_sec = 3;
      tv.tv_usec = 0;
      i = select(width, (void *)&readfds, NULL, NULL, &tv);
      if (i < 0) {
#ifndef NO_SSL
		  BIO_printf(bio_err, "bad read select %d\n", get_last_socket_error());
#else
		  perror("bad read select");
#endif
      }
      else if (i == 0) {
		  printf ("WARNING: selet timed out\n");
		  return -1;
      }
      else if (!FD_ISSET(con->sock, &readfds))
		  break; /* nothing to read */
#ifndef NO_SSL
      k = SSL_read(con->sslcon, &((*sbuf)[*sbuf_len]), sbuf_max_read);
      switch (SSL_get_error(con->sslcon, k))
	{
	case SSL_ERROR_NONE:
	  *sbuf_len += k;
	  sbuf_remain = sbuf_max_read - k;
	  break;
	case SSL_ERROR_WANT_WRITE:
	  if (c_debug)
	    BIO_printf(bio_c_out, "read W BLOCK\n");
	  break;
	case SSL_ERROR_WANT_READ:
	  if (c_debug)
	    BIO_printf(bio_c_out, "read R BLOCK\n");
	  break;
	case SSL_ERROR_WANT_X509_LOOKUP:
	  if (c_debug)
	    BIO_printf(bio_c_out, "read X BLOCK\n");
	  break;
	case SSL_ERROR_SYSCALL:
	  if (c_debug)
	    BIO_printf(bio_c_out, "read:errno=%d\n", get_last_socket_error());
	  return 0;
	case SSL_ERROR_ZERO_RETURN:
	  if (c_debug)
	    BIO_printf(bio_c_out, "closed\n");
	  return 0;
	case SSL_ERROR_SSL:
	  if (c_debug)
	    BIO_printf(bio_c_out, "read: connection closed by peer\n");
	  return 0;
	}
#else

      temp_buf = (char*)malloc(sbuf_max_read);
      k = recv(con->sock, temp_buf, sbuf_max_read, MSG_PEEK); // pre-read check
      size_to_read = checkSizeToRead(temp_buf, k);
      if (size_to_read != 0)
	sbuf_max_read = size_to_read;

      k = read(con->sock, &((*sbuf)[*sbuf_len]), sbuf_max_read);
      *sbuf_len += k;
      sbuf_remain = sbuf_max_read - k;
#endif

      if (recieveFullPacket(*sbuf, *sbuf_len))  //post-read check
	break;
      if (sbuf_remain < sbuf_max_read)
	{
	  *sbuf = (char*)realloc(*sbuf, step * sbuf_max_read);
	  step++;
	}
    }  /* for */
  return 1;
}

/**
 * Checks how many bytes to read from buff.
 * @return number of bytes to read, 0 if cannot determine
 */
#ifdef NO_SSL
static int checkSizeToRead(char *buff, int len)
{
  if (ntohs(*((short*)buff+1)) <= len-8)
    return  (ntohs(*((short*)buff+1)))+8;
  return 0;
}
#endif

/**
 * Checks if the entire SIMCO packet is received.
 * @return 1 if full packet is received, 0 if not
 */
static int recieveFullPacket(char* buff, int len)
{
  if (ntohs(*((short*)buff+1)) == len-8)
    return 1;   /* full packet received */
  return 0;     /* more data to come */
}

/**
 * Disconnects the connection structure of index \a i
 * @param conn connection structure
 */
void midcomAgentDisconnect(MCAgentCon *conn)
{
#ifndef NO_SSL
  SSL_shutdown(conn->sslcon);
  /*SHUTDOWN(SSL_get_fd(conn->sslcon)); */
  if (conn->sslcon != NULL)
    SSL_free(conn->sslcon);
  conn->sslcon = NULL;
#else
  if (conn->sock != -1)
    close(conn->sock);
#endif
  conn->sock = -1;
  if (conn->sbuf != NULL)
      free(conn->sbuf);
  conn->sbuf = NULL;
  conn->sbuf_offset = 0;
  return;
}

/**
 * Disconnects all open connections and frees the SSL_ctx structure.
 */
void mc_close()
{
  int i;

  pthread_mutex_destroy(&myMutex);
  nb_destroy();

  for (i = 0; i < MAX_CON; i++) {
    if (conn[i].sock != -1)
      {
	midcomAgentDisconnect(&conn[i]);
      }
  }

#ifndef NO_SSL
  if (ctx != NULL) SSL_CTX_free(ctx);
  if (bio_c_out != NULL)
    {
      BIO_free(bio_c_out);
      bio_c_out=NULL;
    }

  /* changing from 0.9.7d to 0.9.6.m
  apps_shutdown();
  */
#endif

  init_all = 0;
  return;
}

/**
 * Converts a network address integer to dotted decimal format.
 * @param addr network address integer
 * @param str pointer to char containing dotted decimal address.
 */
void ip_ntoa(unsigned int addr, char *str)
{
  unsigned char *tmp = (char *)&(addr);
  sprintf((str), "%d.%d.%d.%d", *tmp, *(tmp+1), *(tmp+2), *(tmp+3));
}

/**
 * Parses the simco header.
 * @param pdu pointer to the simco header.
 * @param size size of the simco header.
 * @param status status code in the simco header.
 */
void parseSIMCOHeader(char *pdu, int size, int *status)
{
  if ((*pdu == BSC_NEG_RPL_MSG) || (*pdu == BSC_NTF_MSG) || (*pdu == BSC_POS_RPL_MSG))
    *status = *(pdu+1);
  else
    *status = 0;
  return;
}

/**
 * Parses the simco packet to extract an array of rule identifiers.
 * @param pdu pointer to the simco packet.
 * @param size size of the simco packet.
 * @param rules pointer to the rule identifiers array.
 * @param numrules number of rule identifiers int the array.
 * @return 0 on success, or error code set in packet.
 */
int parseSIMCORuleList(char *pdu, int size, int **rules, int *numrules)
{
  short attrlen, total, count;
  char *parse_ptr = pdu;
  if (*parse_ptr == BSC_NEG_RPL_MSG)
    return *(parse_ptr+1);
  total = ntohs(*((short*)parse_ptr+1))/2;
  if (total > *numrules)
    *rules = (int*)realloc(*rules, total*sizeof(int));
  parse_ptr += 8;
  size -= 8;
  count = 0;
  while ((size) && (count < total))
    {
      if (ntohs(*(short*)parse_ptr) == PAY_PRI_ATT)
        {
          attrlen = ntohs(*(short*)(parse_ptr+2));
          if (size < attrlen) break;
          (*rules)[count] = ntohl(*(int*)(parse_ptr+4));
          count++;
        }
      else
        break;
      size -= (attrlen + 4);
      parse_ptr += (attrlen + 4);
    }
  *numrules = count;
  return 0;
}

/**
 * Parses the simco packet passed in \a pdu and fills corresponding fields
 * in \a sp.
 * @param sp pointer to the simco structure which will be filled.
 * @param pdu pointer to the simco packet.
 * @param size size of the simco packet.
 */
void parseSIMCOPdu(simcoPacket_t *sp, char *pdu, int size)
{
  short attrlen;
  char *parse_ptr = pdu;
  printf("\t\t\t Parsing SIMCO packet status = 0x%x size = %d\n", *(parse_ptr+1), size);
  if (*parse_ptr == BSC_NEG_RPL_MSG)
    {
      sp->status = *(parse_ptr+1);
      return;
    }
  else if (*parse_ptr == BSC_NTF_MSG)
    sp->status = *(parse_ptr+1);
  else
    sp->status = 0;
  parse_ptr += 8;
  size -= 8;
  /* memset((void*)sp, 0, sizeof(simcoPacket_t)); */
  while (size)
    {
      if (ntohs(*(short*)parse_ptr) == PAY_PVR_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_ACH_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_ACO_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_MCP_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	  /* sp->policyRuleMaxLifetime = ntohl(*(int*)(parse_ptr+8)); */
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_PRI_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	  sp->ruleId = ntohl(*(int*)(parse_ptr+4));
	  printf("\t\t\t Received RuleId = %d\n ",  sp->ruleId);
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_PGI_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	  sp->groupId = ntohl(*(int*)(parse_ptr+4));
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_PRL_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	  sp->lifetime = ntohl(*(int*)(parse_ptr+4));
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_PRO_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_ADD_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	  if (attrlen == 12)
	    {
	      if (*(parse_ptr+7) == ATA_A0_FLG)
		{
		  sp->internal_ip_mask = (int)*(parse_ptr+5);
		  sp->protocol = (int)*(parse_ptr+6);
		  sp->internal_port = ntohs(*(short*)(parse_ptr+8));
		  sp->internal_port_range = ntohs(*(short*)(parse_ptr+10));
		  ip_ntoa((unsigned int)(*(int*)(parse_ptr+12)), sp->internal_ip);
		}
	      else if (*(parse_ptr+7) == ATA_A1_FLG)
		{
		  sp->inside_ip_mask = (int)*(parse_ptr+5);
		  sp->protocol = (int)*(parse_ptr+6);
		  sp->inside_port = ntohs(*(short*)(parse_ptr+8));
		  sp->inside_port_range = ntohs(*(short*)(parse_ptr+10));
		  ip_ntoa((unsigned int)(*(int*)(parse_ptr+12)), sp->inside_ip);
		}
	      else if (*(parse_ptr+7) == ATA_A2_FLG)
		{
		  sp->outside_ip_mask = (int)*(parse_ptr+5);
		  sp->protocol = (int)*(parse_ptr+6);
		  sp->outside_port = ntohs(*(short*)(parse_ptr+8));
		  sp->outside_port_range = ntohs(*(short*)(parse_ptr+10));
		  ip_ntoa((unsigned int)(*(int*)(parse_ptr+12)), sp->outside_ip);
		}
	      else if (*(parse_ptr+7) == ATA_A3_FLG)
		{
		  sp->external_ip_mask = (int)*(parse_ptr+5);
		  sp->protocol = (int)*(parse_ptr+6);
		  sp->external_port = ntohs(*(short*)(parse_ptr+8));
		  sp->external_port_range = ntohs(*(short*)(parse_ptr+10));
		  ip_ntoa((unsigned int)(*(int*)(parse_ptr+12)), sp->external_ip);
		}
	    }
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_PRR_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	}
      else if (ntohs(*(short*)parse_ptr) == PAY_PER_ATT)
	{
	  attrlen = ntohs(*(short*)(parse_ptr+2));
	  if (size < attrlen) break;
	}
      else
	{
	  printf("parse failing for 0x%hx %d %d!!!!!!!!\n", ntohs(*(short*)parse_ptr), parse_ptr-pdu, size);
	  break;
	}
      size -= (attrlen + 4);
      parse_ptr += (attrlen + 4);
    }
  return;
}

int makePingRequest(char *request)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PNG_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makeSessionEstablishRequest(char *request)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_SE_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_PVR_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_CHAR_INSERT(request, index, SIMCO_MAJOR_VERSION);
  SIMCO_CHAR_INSERT(request, index, SIMCO_MINOR_VERSION);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makeSessionAuthenticateRequest(char *request, const char * str)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_SA_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_ACO_ATT);
  SIMCO_SHORT_INSERT(request, index, strlen(str));
  SIMCO_STRING_INSERT(request, index, str);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makeSessionTerminateRequest(char *request)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_ST_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makePolicyReserveRuleRequest(char *request, const simcoPacket_t *sp)
{
  int index = 0;
  memset((char*)&request[0], 0, sizeof(request));
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PRR_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_PRR_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_CHAR_INSERT(request, index, PRR_TRA_FLG | PRR_PRA_FLG | PRR_IPI4_FLG | PRR_IPO4_FLG);
  SIMCO_CHAR_INSERT(request, index, 0x11); /* UDP */
  if (sp->nat_port_range == 0) {
    SIMCO_SHORT_INSERT(request, index, 0x0001); /* reserve only one port */
  } else {
    SIMCO_SHORT_INSERT(request, index, sp->nat_port_range);
  }
  SIMCO_SHORT_INSERT(request, index, PAY_PRL_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->lifetime);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makePolicyEnableRuleRequest(char *request, const simcoPacket_t *sp)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PER_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_PER_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_CHAR_INSERT(request, index, 0);
  SIMCO_CHAR_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, 0x0000);
  SIMCO_SHORT_INSERT(request, index, PAY_ADD_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x000c);
  SIMCO_CHAR_INSERT(request, index, 0x01);
  SIMCO_CHAR_INSERT(request, index, 0x20);
  SIMCO_CHAR_INSERT(request, index, 0x11);
  SIMCO_CHAR_INSERT(request, index, 0x00);
  SIMCO_SHORT_INSERT(request, index, sp->internal_port);
  SIMCO_SHORT_INSERT(request, index, 0x0001);
  SIMCO_INT_INSERT(request, index, htonl(inet_addr(sp->internal_ip)));
  SIMCO_SHORT_INSERT(request, index, PAY_ADD_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x000c);
  SIMCO_CHAR_INSERT(request, index, 0x01);
  SIMCO_CHAR_INSERT(request, index, 0x20);
  SIMCO_CHAR_INSERT(request, index, 0x11);
  SIMCO_CHAR_INSERT(request, index, 0x03);
  SIMCO_SHORT_INSERT(request, index, sp->external_port);
  SIMCO_SHORT_INSERT(request, index, 0x0001);
  SIMCO_INT_INSERT(request, index, htonl(inet_addr(sp->external_ip)));
  SIMCO_SHORT_INSERT(request, index, PAY_PRL_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->lifetime);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makePERAfterReservationRequest(char *request, const simcoPacket_t *sp)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PEA_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_PER_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
#if 0
  SIMCO_CHAR_INSERT(request, index, 0);
  SIMCO_CHAR_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, 0x0000);
#else
  SIMCO_INT_INSERT(request, index, sp->codec);
#endif
  SIMCO_SHORT_INSERT(request, index, PAY_ADD_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x000c);
  SIMCO_CHAR_INSERT(request, index, 0x01);
  SIMCO_CHAR_INSERT(request, index, 0x20);
  SIMCO_CHAR_INSERT(request, index, 0x11);
  SIMCO_CHAR_INSERT(request, index, 0x00);
  SIMCO_SHORT_INSERT(request, index, sp->internal_port);
  SIMCO_SHORT_INSERT(request, index, 0x0001);
  SIMCO_INT_INSERT(request, index, htonl(inet_addr(sp->internal_ip)));
  SIMCO_SHORT_INSERT(request, index, PAY_ADD_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x000c);
  SIMCO_CHAR_INSERT(request, index, 0x01);
  SIMCO_CHAR_INSERT(request, index, 0x20);
  SIMCO_CHAR_INSERT(request, index, 0x11);
  SIMCO_CHAR_INSERT(request, index, 0x03);
  SIMCO_SHORT_INSERT(request, index, sp->external_port);
  SIMCO_SHORT_INSERT(request, index, 0x0001);
  SIMCO_INT_INSERT(request, index, htonl(inet_addr(sp->external_ip)));
  SIMCO_SHORT_INSERT(request, index, PAY_PRL_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->lifetime);
  SIMCO_SHORT_INSERT(request, index, PAY_PRI_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->ruleId);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makePBRAfterReservationRequest(char *request, const simcoPacket_t *sp)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PBA_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_PER_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
#if 0
  SIMCO_CHAR_INSERT(request, index, 0);
  SIMCO_CHAR_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, 0x0000);
#else
  SIMCO_INT_INSERT(request, index, sp->codec);
#endif
  SIMCO_SHORT_INSERT(request, index, PAY_ADD_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x000c);
  SIMCO_CHAR_INSERT(request, index, 0x01);
  SIMCO_CHAR_INSERT(request, index, 0x20);
  SIMCO_CHAR_INSERT(request, index, 0x11);
  SIMCO_CHAR_INSERT(request, index, 0x00);
  SIMCO_SHORT_INSERT(request, index, sp->internal_port);
  SIMCO_SHORT_INSERT(request, index, 0x0001);
  SIMCO_INT_INSERT(request, index, htonl(inet_addr(sp->internal_ip)));
  SIMCO_SHORT_INSERT(request, index, PAY_ADD_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x000c);
  SIMCO_CHAR_INSERT(request, index, 0x01);
  SIMCO_CHAR_INSERT(request, index, 0x20);
  SIMCO_CHAR_INSERT(request, index, 0x11);
  SIMCO_CHAR_INSERT(request, index, 0x03);
  SIMCO_SHORT_INSERT(request, index, sp->external_port);
  SIMCO_SHORT_INSERT(request, index, 0x0001);
  SIMCO_INT_INSERT(request, index, htonl(inet_addr(sp->external_ip)));
  SIMCO_SHORT_INSERT(request, index, PAY_PRL_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->lifetime);
  SIMCO_SHORT_INSERT(request, index, PAY_PRI_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->ruleId);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makePolicyRuleLifetimeChangeRequest(char *request, const simcoPacket_t *sp)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PLC_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_PRI_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->ruleId);
  SIMCO_SHORT_INSERT(request, index, PAY_PRL_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->lifetime);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makePolicyRuleStatusRequest(char *request, const simcoPacket_t *sp)
{
  int index = 0;
  memset((char*)&request[0], 0, sizeof(request));
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PS_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SHORT_INSERT(request, index, PAY_PRI_ATT);
  SIMCO_SHORT_INSERT(request, index, 0x0004);
  SIMCO_INT_INSERT(request, index, sp->ruleId);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}

int makePolicyRuleListRequest(char *request)
{
  int index = 0;
  SIMCO_CHAR_INSERT(request, index, BSC_REQ_MSG);
  SIMCO_CHAR_INSERT(request, index, SUB_PL_MSG);
  SIMCO_SHORT_INSERT(request, index, 0);
  SIMCO_INT_INSERT(request, index, 0);
  SIMCO_SIZE_INSERT(request, index-8);
  return index;
}


