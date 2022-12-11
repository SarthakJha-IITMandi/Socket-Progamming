#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/*
 * Desired design of maximum size and alignment
 */
#define _SS_MAXSIZE 128 /* Implementation specific max size */
#define _SS_ALIGNSIZE (sizeof(int64_t))
/* Implementation specific desired alignment */
/*
 * Definitions used for sockaddr_storage structure paddings design.
 */
#define _SS_PAD1SIZE (_SS_ALIGNSIZE - sizeof(sa_family_t))
#define _SS_PAD2SIZE (_SS_MAXSIZE - (sizeof(sa_family_t) + _SS_PAD1SIZE + _SS_ALIGNSIZE))

struct addrinfo
{
    int ai_flags;             // AI_PASSIVE, AI_CANONNAME, etc.
    int ai_family;            // AF_INET, AF_INET6, AF_UNSPEC
    int ai_socktype;          // SOCK_STREAM, SOCK_DGRAM
    int ai_protocol;          // use 0 for "any"
    size_t ai_addrlen;        // size of ai_addr in bytes
    struct sockaddr *ai_addr; // struct sockaddr_in or _in6
    char *ai_canonname;       // full canonical hostname
    struct addrinfo *ai_next; // linked list, next node
};

struct sockaddr
{
    unsigned short sa_family; // address family, AF_xxx
    char sa_data[14];         // 14 bytes of protocol address
};

// (IPv4 only--see struct sockaddr_in6 for IPv6)
struct sockaddr_in
{
    short int sin_family;        // Address family, AF_INET
    unsigned short int sin_port; // Port number
    struct in_addr sin_addr;     // Internet address
    unsigned char sin_zero[8];   // Same size as struct sockaddr
};

// (IPv4 only--see struct in6_addr for IPv6)
struct in_addr
{
    uint32_t s_addr; // that's a 32-bit int (4 bytes)
};

// (IPv6 only--see struct sockaddr_in and struct in_addr for IPv4)
struct sockaddr_in6
{
    u_int16_t sin6_family;     // address family, AF_INET6
    u_int16_t sin6_port;       // port number, Network Byte Order
    u_int32_t sin6_flowinfo;   // IPv6 flow information
    struct in6_addr sin6_addr; // IPv6 address
    u_int32_t sin6_scope_id;   // Scope ID
};

// (IPv6 only--see struct sockaddr_in and struct in_addr for IPv4)
struct in6_addr
{
    unsigned char si6_addr[16]; // IPv6 address
};

// sockaddr_storage is used if ipv4/6 not known in advance
struct sockaddr_storage
{
    sa_family_t ss_family; /* address family */
    /* Following fields are implementation specific */
    char _ss_pad1[_SS_PAD1SIZE];
    /* 6 byte pad, this is to make implementation */
    /* specific pad up to alignment field that */
    /* follows explicit in the data structure */
    int64_t _ss_align; /* field to force desired structure */
                       /* storage alignment */
    char _ss_pad2[_SS_PAD2SIZE];
    /* 112 byte pad to achieve desired size, */
    /* _SS_MAXSIZE value minus size of ss_family */
    /* __ss_pad1, __ss_align fields is 112 */
};
