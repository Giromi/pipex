#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#define MAX_BUF 1024
#define	CHILD	0
#define	MSG_PARENT			"message form parent"
#define HEAD_PARENT			"child	: "
#define HEAD_CHILD			"parent	: "
enum e_pipe
{
	A		=	0,
	B		=	1,
	READ	=	0,
	WRITE	=	1
};


int	fd_putchar(int fd, int	c)
{
	return (write(fd, &c, 1));
}

void	fd_putstr_once(int fd, char *s)
{
	int	res;

	res = write(fd, s, strlen(s));
}

int	fd_putstr(int fd, char *s)
{
	int	i;
	
	i = 0;
	while (s[i])
		fd_putchar(s[i++], fd);
	return (i);
}

void print_error(char *str)
{
	write(STDERR_FILENO, str, strlen(str));
	exit(1);
}

int	my_pipe(int	fildes[2])
{
	int	res;

	res = pipe(fildes);
	if (res < 0)
		print_error("error: pipe");
	return (res);
}

pid_t	my_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		print_error("error: pipe");
	return (pid);
}

void	putmsg(char *head, char *out)
{
	fd_putstr(STDOUT_FILENO, head);
	fd_putstr(STDOUT_FILENO, out);
	fd_putchar(STDOUT_FILENO, '\n');
}

int	main(void)
{
	int		fd[2][2];
	pid_t	pid;
	char	buf[MAX_BUF];
	int		len;
	int		cnt;

	signal(SIGPIPE, SIG_IGN);
	cnt = my_pipe(fd[A]);
	cnt = my_pipe(fd[B]);
	pid = my_fork();
	if (!pid) // child
	{
		close(fd[A][WRITE]);
		close(fd[B][READ]);
		read(fd[A][READ], buf, MAX_BUF);
		// putmsg(HEAD_CHILD, buf);
		strcat(buf, " + child\n");
		write(fd[B][WRITE], buf, strlen(buf));
	}
	else
	{
		close(fd[A][READ]);
		close(fd[B][WRITE]);
		strcpy(buf, MSG_PARENT);
		write(fd[A][WRITE], buf, strlen(buf));
		read(fd[B][READ], buf, MAX_BUF);

		// result
		fd_putstr(STDOUT_FILENO, buf);
	}
	return (0);
}

// int	main(void)
// {
//     int		fd[2][2];
//     pid_t	pid;
//     char	buf[MAX_BUF];
//     int		len;
//
//     my_pipe(fd[A]);
//     my_pipe(fd[B]);
//     pid = my_fork();
//     if (!pid) // child
//     {
//         close(fd[A][WRITE]);
//         close(fd[B][READ]);
//         read(fd[A][READ], buf, MAX_BUF);
//         // putmsg(HEAD_CHILD, buf);
//         strcat(buf, " + child\n");
//         fd_putstr_once(fd[B][WRITE], buf);
//     }
//     else
//     {
//         close(fd[A][READ]);
//         close(fd[B][WRITE]);
//         strcpy(buf, MSG_PARENT);
//         fd_putstr_once(fd[A][WRITE], buf);
//         read(fd[B][READ], buf, MAX_BUF);
//
//         // result
//         fd_putstr(STDOUT_FILENO, buf);
//     }
//     return (0);
// }
