#include <stdlib.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>

int sock_connect_to_serv(char *adress, int port, int fd, struct sockaddr_in *sa);
int sock_net_new(int *fd);
int sock_select(int fd, fd_set *read_set);
/* Accept connection and set fds if not NULL, else respond that no socket available */
int sock_accept(struct pollfd *fds, int max_clients);
int sock_init_poll(struct pollfd *fds, int port, int max_clients);
void sock_close_poll(struct pollfd *fds, int max_clients);


