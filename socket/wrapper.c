#define LISTENQ  1024  /* Second argument to listen() */

#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;
typedef struct sockaddr SA;

void print_error(char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

void print_error_posix(int code, char *msg) {
    fprintf(stderr, "%s: %s\n", msg, strerror(code));
    exit(0);
}

int open_listenfd(int port) {

    int listenfd, optval=1;
    struct sockaddr_in serveraddr;
  
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
	    return -1;
    }
 
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, 
		   (const void *)&optval , sizeof(int)) < 0) {
	    return -1;
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET; 
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serveraddr.sin_port = htons((unsigned short)port); 

    if (bind(listenfd, (SA *)&serveraddr, sizeof(serveraddr)) < 0) {
	    return -1;
    }

    if (listen(listenfd, LISTENQ) < 0) {
	    return -1;
    }

    return listenfd;

}

int Open_listenfd(int port) {
    int rc;

    if ((rc = open_listenfd(port)) < 0) {
	    print_error("Open_listenfd error");
    }
    return rc;
}

void *Malloc(size_t size) {
    void *p;

    if ((p  = malloc(size)) == NULL) {
	    print_error("Malloc error");
    }
    return p;
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) {
    int rc;

    if ((rc = accept(s, addr, addrlen)) < 0) {
	    print_error("Accept error");
    }
    return rc;
}

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp, 
		    void * (*routine)(void *), void *argp) {
    int rc;

    if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0) {
	    print_error_posix(rc, "Pthread_create error");
    }
}

void Pthread_detach(pthread_t tid) {
    int rc;

    if ((rc = pthread_detach(tid)) != 0) {
	    print_error_posix(rc, "Pthread_detach error");
    }
}

pthread_t Pthread_self(void) {
    return pthread_self();
}

void Free(void *ptr) {
    free(ptr);
}

void Close(int fd) {
    int rc;

    if ((rc = close(fd)) < 0) {
        print_error("Close error");
    }
}

void rio_readinitb(rio_t *rp, int fd) {
    rp->rio_fd = fd;  
    rp->rio_cnt = 0;  
    rp->rio_bufptr = rp->rio_buf;
}

void Rio_readinitb(rio_t *rp, int fd) {
    rio_readinitb(rp, fd);
} 

ssize_t rio_writen(int fd, void *usrbuf, size_t n) {

    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) <= 0) {
            if (errno == EINTR)  {
                nwritten = 0;
            } else {
                return -1;
            }
        }

        nleft -= nwritten;
        bufp += nwritten;

    }

    return n;

}
void Rio_writen(int fd, void *usrbuf, size_t n) {
    if (rio_writen(fd, usrbuf, n) != n) {
        print_error("Rio_writen error");
    }
}

