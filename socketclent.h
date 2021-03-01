#ifndef _SOCKETCLENT_H
#endif  _SOCKETCLENT_H

#ifdef __cplusplus
extern "C"{
#endif

//1.

//socket client initial
int socketclient_init(void **handle);

//sockte client send
int socketclient_send(void *handle, unsigned char *buf, int buflen);

//socket client receive
int socketclient_recv(void *handle, unsigned char *buf, int *buflen);

//socket client destroy
int socketclient_destroy(void *handle);

//2.

//socket client initial 
int socketclient_init2(void **handle);

//socket client send
int socketclient_send2(void *handle, unsigned char *buf, int buflen);

//socket client receive 
int socketclient_recv2(void *handle, unsigned char **buf, int *buflen);
int socketclient_free(unsigned char **buf);

//socket client destroy
int socketcleint_destroy2(void **handle);

#ifdef __cplusplus
}
#endif

#endif