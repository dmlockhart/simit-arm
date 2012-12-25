/***************************************************************************
						  syscall.c  -  description
							 -------------------
	begin				: Wed Sep 26 2001
	copyright			: (C) 2001 CEA and Université Paris XI Orsay
	author			   : Gilles Mouchard
	email				: gilles.mouchard@lri.fr, gilles.mouchard@cea.fr
 ***************************************************************************/

/***************************************************************************
 *																		 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or	 *
 *   (at your option) any later version.								   *
 *																		 *
 ***************************************************************************/

/* The file is modified by Wei Qin for arm target.
 * Added mmap syscall interpretation.
 */
/*
 * Modified by Chris Han for better Linux compatibility
 */

#ifdef linux
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#define EINVAL 22
#include <misc.h>
#include "syscall.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/times.h>
#include <signal.h>

#ifdef COMPILE_SIMULATOR
#include "armsim.hpp"

#define SYSCALL_STAGE_0			if (emu->syscall_phase==0)
#define SYSCALL_STAGE_1			else if (emu->syscall_phase==1)
#define SYSCALL_STAGE_2			else if (emu->syscall_phase==2)
#define SYSCALL_STAGE_3			else if (emu->syscall_phase==3)
#define SYSCALL_RESET_STAGE		emu->syscall_phase = 0;
#define SYSCALL_ADV_STAGE		emu->syscall_phase++; return true;
#define SYSCALL_ENDOF_STAGE		return true;
#define SYSCALL_STAGE_FALSE		else {assert(0); return false;}

#define MEM_WRITE_BLOCK(_a, _b, _s) \
	emu->syscall_mem_write_block(_a, (uint8_t*)_b, _s)
#define MEM_READ_BLOCK(_a, _b, _s) \
	emu->syscall_mem_read_block(_a, (uint8_t*)_b, _s)

#else

#include <armemul.h>
#include "armmmu.h"

#define SYSCALL_STAGE_0
#define SYSCALL_STAGE_1
#define SYSCALL_STAGE_2
#define SYSCALL_STAGE_3
#define SYSCALL_RESET_STAGE
#define SYSCALL_ADV_STAGE
#define SYSCALL_ENDOF_STAGE
#define SYSCALL_STAGE_FALSE	return true;

/*#define MEM_WRITE_BLOCK(_a, _b, _s) emu->mmu->write_block(emu,_a, _b, _s)
#define MEM_READ_BLOCK(_a, _b, _s)  emu->mmu->read_block(emu,_b, _a, _s)*/
#define MEM_WRITE_BLOCK(_a, _b, _s) emu->mem->write_block(_a, _b, _s)
#define MEM_READ_BLOCK(_a, _b, _s)  emu->mem->read_block(_b, _a, _s)

#endif

#define __SYSCALL_exit		  1
#define __SYSCALL_fork		  2
#define __SYSCALL_read		  3
#define __SYSCALL_write		  4
#define __SYSCALL_open		  5
#define __SYSCALL_close		  6
#define __SYSCALL_waitpid		  7
#define __SYSCALL_creat		  8
#define __SYSCALL_link		  9
#define __SYSCALL_unlink		 10
#define __SYSCALL_execve		 11
#define __SYSCALL_chdir		 12
#define __SYSCALL_time		 13
#define __SYSCALL_mknod		 14
#define __SYSCALL_chmod		 15
#define __SYSCALL_lchown		 16
#define __SYSCALL_break		 17
#define __SYSCALL_oldstat		 18
#define __SYSCALL_lseek		 19
#define __SYSCALL_getpid		 20
#define __SYSCALL_mount		 21
#define __SYSCALL_umount		 22
#define __SYSCALL_setuid		 23
#define __SYSCALL_getuid		 24
#define __SYSCALL_stime		 25
#define __SYSCALL_ptrace		 26
#define __SYSCALL_alarm		 27
#define __SYSCALL_oldfstat		 28
#define __SYSCALL_pause		 29
#define __SYSCALL_utime		 30
#define __SYSCALL_stty		 31
#define __SYSCALL_gtty		 32
#define __SYSCALL_access		 33
#define __SYSCALL_nice		 34
#define __SYSCALL_ftime		 35
#define __SYSCALL_sync		 36
#define __SYSCALL_kill		 37
#define __SYSCALL_rename		 38
#define __SYSCALL_mkdir		 39
#define __SYSCALL_rmdir		 40
#define __SYSCALL_dup		 41
#define __SYSCALL_pipe		 42
#define __SYSCALL_times		 43
#define __SYSCALL_prof		 44
#define __SYSCALL_brk		 45
#define __SYSCALL_setgid		 46
#define __SYSCALL_getgid		 47
#define __SYSCALL_signal		 48
#define __SYSCALL_geteuid		 49
#define __SYSCALL_getegid		 50
#define __SYSCALL_acct		 51
#define __SYSCALL_umount2		 52
#define __SYSCALL_lock		 53
#define __SYSCALL_ioctl		 54
#define __SYSCALL_fcntl		 55
#define __SYSCALL_mpx		 56
#define __SYSCALL_setpgid		 57
#define __SYSCALL_ulimit		 58
#define __SYSCALL_oldolduname	 59
#define __SYSCALL_umask		 60
#define __SYSCALL_chroot		 61
#define __SYSCALL_ustat		 62
#define __SYSCALL_dup2		 63
#define __SYSCALL_getppid		 64
#define __SYSCALL_getpgrp		 65
#define __SYSCALL_setsid		 66
#define __SYSCALL_sigaction		 67
#define __SYSCALL_sgetmask		 68
#define __SYSCALL_ssetmask		 69
#define __SYSCALL_setreuid		 70
#define __SYSCALL_setregid		 71
#define __SYSCALL_sigsuspend		 72
#define __SYSCALL_sigpending		 73
#define __SYSCALL_sethostname	 74
#define __SYSCALL_setrlimit		 75
#define __SYSCALL_getrlimit		 76
#define __SYSCALL_getrusage		 77
#define __SYSCALL_gettimeofday	 78
#define __SYSCALL_settimeofday	 79
#define __SYSCALL_getgroups		 80
#define __SYSCALL_setgroups		 81
#define __SYSCALL_select		 82
#define __SYSCALL_symlink		 83
#define __SYSCALL_oldlstat		 84
#define __SYSCALL_readlink		 85
#define __SYSCALL_uselib		 86
#define __SYSCALL_swapon		 87
#define __SYSCALL_reboot		 88
#define __SYSCALL_readdir		 89
#define __SYSCALL_mmap		 90
#define __SYSCALL_munmap		 91
#define __SYSCALL_truncate		 92
#define __SYSCALL_ftruncate		 93
#define __SYSCALL_fchmod		 94
#define __SYSCALL_fchown		 95
#define __SYSCALL_getpriority	 96
#define __SYSCALL_setpriority	 97
#define __SYSCALL_profil		 98
#define __SYSCALL_statfs		 99
#define __SYSCALL_fstatfs		100
#define __SYSCALL_ioperm		101
#define __SYSCALL_socketcall		102
#define __SYSCALL_syslog		103
#define __SYSCALL_setitimer		104
#define __SYSCALL_getitimer		105
#define __SYSCALL_stat		106
#define __SYSCALL_lstat		107
#define __SYSCALL_fstat		108
#define __SYSCALL_olduname		109
#define __SYSCALL_iopl		110
#define __SYSCALL_vhangup		111
#define __SYSCALL_idle		112
#define __SYSCALL_vm86old		113
#define __SYSCALL_wait4		114
#define __SYSCALL_swapoff		115
#define __SYSCALL_sysinfo		116
#define __SYSCALL_ipc		117
#define __SYSCALL_fsync		118
#define __SYSCALL_sigreturn		119
#define __SYSCALL_clone		120
#define __SYSCALL_setdomainname	121
#define __SYSCALL_uname		122
#define __SYSCALL_modify_ldt		123
#define __SYSCALL_adjtimex		124
#define __SYSCALL_mprotect		125
#define __SYSCALL_sigprocmask	126
#define __SYSCALL_create_module	127
#define __SYSCALL_init_module	128
#define __SYSCALL_delete_module	129
#define __SYSCALL_get_kernel_syms	130
#define __SYSCALL_quotactl		131
#define __SYSCALL_getpgid		132
#define __SYSCALL_fchdir		133
#define __SYSCALL_bdflush		134
#define __SYSCALL_sysfs		135
#define __SYSCALL_personality	136
#define __SYSCALL_afs_syscall	137
#define __SYSCALL_setfsuid		138
#define __SYSCALL_setfsgid		139
#define __SYSCALL__llseek		140
#define __SYSCALL_getdents		141
#define __SYSCALL__newselect		142
#define __SYSCALL_flock		143
#define __SYSCALL_msync		144
#define __SYSCALL_readv		145
#define __SYSCALL_writev		146
#define __SYSCALL_getsid		147
#define __SYSCALL_fdatasync		148
#define __SYSCALL__sysctl		149
#define __SYSCALL_mlock		150
#define __SYSCALL_munlock		151
#define __SYSCALL_mlockall		152
#define __SYSCALL_munlockall		153
#define __SYSCALL_sched_setparam		154
#define __SYSCALL_sched_getparam		155
#define __SYSCALL_sched_setscheduler		156
#define __SYSCALL_sched_getscheduler		157
#define __SYSCALL_sched_yield		158
#define __SYSCALL_sched_get_priority_max	159
#define __SYSCALL_sched_get_priority_min	160
#define __SYSCALL_sched_rr_get_interval	161
#define __SYSCALL_nanosleep		162
#define __SYSCALL_mremap		163
#define __SYSCALL_setresuid		164
#define __SYSCALL_getresuid		165
#define __SYSCALL_vm86		166
#define __SYSCALL_query_module	167
#define __SYSCALL_poll		168
#define __SYSCALL_nfsservctl		169
#define __SYSCALL_setresgid		170
#define __SYSCALL_getresgid		171
#define __SYSCALL_prctl			  172
#define __SYSCALL_rt_sigreturn	173
#define __SYSCALL_rt_sigaction	174
#define __SYSCALL_rt_sigprocmask	175
#define __SYSCALL_rt_sigpending	176
#define __SYSCALL_rt_sigtimedwait	177
#define __SYSCALL_rt_sigqueueinfo	178
#define __SYSCALL_rt_sigsuspend	179
#define __SYSCALL_pread		180
#define __SYSCALL_pwrite		181
#define __SYSCALL_chown		182
#define __SYSCALL_getcwd		183
#define __SYSCALL_capget		184
#define __SYSCALL_capset		185
#define __SYSCALL_sigaltstack	186
#define __SYSCALL_sendfile		187
#define __SYSCALL_getpmsg		188
#define __SYSCALL_putpmsg		189
#define __SYSCALL_vfork		190
#define __SYSCALL_ugetrlimit		191
#define __SYSCALL_mmap2		192
#define __SYSCALL_truncate64		193
#define __SYSCALL_ftruncate64	194
#define __SYSCALL_stat64		195
#define __SYSCALL_lstat64		196
#define __SYSCALL_fstat64		197
#define __SYSCALL_lchown32		198
#define __SYSCALL_getuid32		199
#define __SYSCALL_getgid32		200
#define __SYSCALL_geteuid32		201
#define __SYSCALL_getegid32		202
#define __SYSCALL_setreuid32		203
#define __SYSCALL_setregid32		204
#define __SYSCALL_getgroups32	205
#define __SYSCALL_setgroups32	206
#define __SYSCALL_fchown32		207
#define __SYSCALL_setresuid32	208
#define __SYSCALL_getresuid32	209
#define __SYSCALL_setresgid32	210
#define __SYSCALL_getresgid32	211
#define __SYSCALL_chown32		212
#define __SYSCALL_setuid32		213
#define __SYSCALL_setgid32		214
#define __SYSCALL_setfsuid32		215
#define __SYSCALL_setfsgid32		216
#define __SYSCALL_pivot_root		217
#define __SYSCALL_mincore		218
#define __SYSCALL_madvise		219
#define __SYSCALL_getdents64		220
#define __SYSCALL_fcntl64		221


#define swap (1-WORDS_BIGENDIAN)

void sc_impl(armulator *emu, arm_inst_t inst)
{
	if(emu->is_user_level())
		do_syscall(emu, inst);
}

#define SYSCALL_NUM ((inst<<8)>>8)


#define PARM(i) READ_REG(i)
#define RETURN(x) WRITE_REG(0, (word_t)(x))

static void arm_swap(void *buf, int count)
{
	if(count > 0)
	{
		char temp[8];
		char *src = (char *) buf + count - 1;
		char *dst = temp;
		
		do
		{
			*dst = *src;
		} while(src--, dst++, --count);
	}
}


bool arm_syscall_exit(armulator *emu)
{
	int ret = PARM(0);
	if (emu->is_verbose())
		fprintf(stderr, "exit(%d)\n", ret);

	emu->prog_exit(ret);
	return true;
}

bool arm_syscall_fork(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_read(armulator *emu)
{
	
SYSCALL_STAGE_0
  {
	int fd;
	size_t count;
	target_addr_t buf_addr;

	fd = PARM(0);
	buf_addr = PARM(1);
	count = (size_t) PARM(2);

	if (emu->is_verbose())
		fprintf(stderr, "read(fd=%d, buf=0x%08x, count=%d)\n",
			fd, buf_addr, (int)count);
		
	emu->syscall_alloc_buf(count);
	emu->syscall_iret = read(fd, emu->syscall_buf, count);
	if(emu->syscall_iret > 0) {
		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, emu->syscall_iret);
	}

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_1
  {
	SYSCALL_RESET_STAGE

	RETURN(emu->syscall_iret);
	return emu->syscall_iret != -1;
  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_write(armulator *emu)
{

	int fd;
	size_t count;

SYSCALL_STAGE_0
  {
	target_addr_t buf_addr;

	fd = PARM(0);
	buf_addr = PARM(1);
	count = (size_t) PARM(2);
	if (emu->is_verbose())
		fprintf(stderr, "write(fd=%d, buf=0x%08x, count=%d)\n",
			fd, buf_addr, (int)count);

	emu->syscall_alloc_buf(count);
	MEM_READ_BLOCK(buf_addr, emu->syscall_buf, count);

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_1
  {
	ssize_t ret;

	SYSCALL_RESET_STAGE

	fd = PARM(0);
	count = (size_t) PARM(2);
	ret = write(fd, emu->syscall_buf, count);

	RETURN(ret);
	return ret != -1;
  }

SYSCALL_STAGE_FALSE

}

static bool arm_syscall_strdup(armulator *emu)
{
SYSCALL_STAGE_0
  {

	/* align to 32byte boundary so as to minimize the risk of
	 * crossing page boundary and trigger page fault.
	 */
	emu->syscall_mem_size = 32 - (emu->syscall_open_addr&31);
	emu->syscall_alloc_buf(emu->syscall_mem_size);
	MEM_READ_BLOCK(emu->syscall_open_addr,
				emu->syscall_buf, emu->syscall_mem_size);
	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_1
  {

	bool done = false;

	while (!done) {

		char *p = (char *)emu->syscall_buf + emu->syscall_open_len;

		while(emu->syscall_mem_size > 0 && !done)
		{
			if(*p == 0) {
				done = true;
				SYSCALL_ADV_STAGE
			}
			emu->syscall_open_len++;
			emu->syscall_mem_size--;
			p++;
		}

		if (!done) {
			emu->syscall_realloc_buf(emu->syscall_open_len+32);
			emu->syscall_mem_size = 32;
			MEM_READ_BLOCK(emu->syscall_open_addr + emu->syscall_open_len,
				(char *)emu->syscall_buf + emu->syscall_open_len, 32);
		}
		SYSCALL_ENDOF_STAGE
	}

  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_open(armulator *emu)
{

SYSCALL_STAGE_0
  {
	emu->syscall_open_addr = PARM(0);
	emu->syscall_open_len = 0;
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }

#ifdef COMPILE_SIMULATOR // emulator do not call twice
SYSCALL_STAGE_1
  {
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }
#endif

SYSCALL_STAGE_2
  {
	int flags;
	mode_t mode;
	int ret;

	flags = PARM(1);
	mode = PARM(2);

	if (emu->is_verbose())
		fprintf(stderr, "open(pathname=\"%s\", flags=%d, mode=%d)\n",
			(char *)emu->syscall_buf, flags, (int)mode);

	ret = open((char *)emu->syscall_buf, flags, mode);

	SYSCALL_RESET_STAGE

	RETURN(ret);
	return ret != -1;
  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_close(armulator *emu)
{
	int fd;
	int ret;
	
	fd = PARM(0);
	if (emu->is_verbose())
		fprintf(stderr, "close(fd=%d)\n", fd);
	ret = close(fd);
	RETURN(ret);
	return ret != -1;
}

bool arm_syscall_waitpid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_creat(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_link(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_unlink(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_execve(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_chdir(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_time(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mknod(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_chmod(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_lchown(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_break(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_oldstat(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_lseek(armulator *emu)
{
	int fildes;
	off_t offset;
	int whence;
	off_t ret;
	
	fildes = PARM(0);
	offset = PARM(1);
	whence = PARM(2);
	if (emu->is_verbose())
		fprintf(stderr, "lseek(fd=%d, offset=%lu, whence=%d)\n", fildes, offset, whence);
	ret = lseek(fildes, offset, whence);
	RETURN(ret);
	return ret != -1;
}

bool arm_syscall_getpid(armulator *emu)
{
	pid_t pid;
	
	if (emu->is_verbose())
		fprintf(stderr, "getpid()\n");
	pid = getpid();
	RETURN(pid);
	return true;
}

bool arm_syscall_mount(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_umount(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setuid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getuid(armulator *emu)
{
	uid_t uid;
	
	if (emu->is_verbose())
		fprintf(stderr, "getuid()\n");
	uid = getuid();
	RETURN(uid);
	return true;
}

bool arm_syscall_stime(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ptrace(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_alarm(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_oldfstat(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_pause(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_utime(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_stty(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_gtty(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_nice(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ftime(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sync(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_kill(armulator *emu) { RETURN(0); return false; }
bool arm_syscall_rename(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mkdir(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_rmdir(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_dup(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_pipe(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_access(armulator *emu) {

SYSCALL_STAGE_0
  {
	emu->syscall_open_addr = PARM(0);
	emu->syscall_open_len = 0;
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }

#ifdef COMPILE_SIMULATOR // emulator do not call twice
SYSCALL_STAGE_1
  {
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }
#endif

SYSCALL_STAGE_2
  {
	int mode;
	int ret;

	mode = PARM(1);

	if (emu->is_verbose())
		fprintf(stderr, "access(pathname=\"%s\", mode=%d)\n",
			(char *)emu->syscall_buf, mode);

	ret = access((char *)emu->syscall_buf, mode);

	SYSCALL_RESET_STAGE

	RETURN(ret);
	return ret != -1;
  }

SYSCALL_STAGE_FALSE
}


static void arm_swap_tms(struct tms *buf)
{
	arm_swap(&buf->tms_utime, sizeof(&buf->tms_utime));
	arm_swap(&buf->tms_stime, sizeof(&buf->tms_stime));
	arm_swap(&buf->tms_cutime, sizeof(&buf->tms_cutime));
	arm_swap(&buf->tms_cstime, sizeof(&buf->tms_cstime));
}

bool arm_syscall_times(armulator *emu)
{

SYSCALL_STAGE_0
  {
	target_addr_t buf_addr;
	struct tms buf;

	buf_addr = PARM(0);

	emu->syscall_alloc_buf(sizeof(struct tms));

	emu->syscall_times_ret = times(&buf);

	if (emu->syscall_times_ret!=(clock_t)-1) {
		if(swap) arm_swap_tms(&buf);
		memcpy(emu->syscall_buf, &buf, sizeof(struct tms));
		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, sizeof(struct tms));
	}
    SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_1
  {
	SYSCALL_RESET_STAGE
	RETURN(emu->syscall_times_ret);
	return emu->syscall_times_ret != (clock_t) -1;
  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_prof(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_brk(armulator *emu)
{
	word_t new_brk_point;
	bool success;
	
	new_brk_point = PARM(0);
	success = true;/*false*/;
	if (emu->is_verbose())
		fprintf(stderr, "brk(end=0x%08x)\n", new_brk_point);
	if(new_brk_point > emu->syscall_get_brk())
	{
		emu->syscall_set_brk(new_brk_point);
		success = true;
	}
	
	RETURN(emu->syscall_get_brk());
	return success;
}

bool arm_syscall_setgid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getgid(armulator *emu)
{
	gid_t gid;
	if (emu->is_verbose())
		fprintf(stderr, "getgid()\n");
	gid = getgid();
	RETURN(gid);
	return true;
}

bool arm_syscall_signal(armulator *emu)
{
	RETURN(-EINVAL);
	return false;
}

bool arm_syscall_geteuid(armulator *emu)
{
	uid_t uid;
	
	if (emu->is_verbose())
		fprintf(stderr, "geteuid()\n");
	uid = geteuid();
	RETURN(uid);
	return true;
}

bool arm_syscall_getegid(armulator *emu)
{
	gid_t gid;
	
	if (emu->is_verbose())
		fprintf(stderr, "getegid()\n");
	gid = getegid();
	RETURN(gid);
	return true;
}

bool arm_syscall_acct(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_umount2(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_lock(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ioctl(armulator *emu) { 	RETURN(-EINVAL); return false; }
bool arm_syscall_fcntl(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mpx(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setpgid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ulimit(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_oldolduname(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_umask(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_chroot(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ustat(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_dup2(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getppid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getpgrp(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setsid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sigaction(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sgetmask(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ssetmask(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setreuid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setregid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sigsuspend(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sigpending(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sethostname(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setrlimit(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getrlimit(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getrusage(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_gettimeofday(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_settimeofday(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getgroups(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setgroups(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_select(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_symlink(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_oldlstat(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_readlink(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_uselib(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_swapon(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_reboot(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_readdir(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_munmap(armulator *emu)
{
#if 0
	size_t u = (size_t)PARM(1);
	
	if (emu->syscall_get_brk() - u > emu->syscall_get_brk())
		goto err;
	
	if (emu->syscall_get_brk() - u < MMAP_BASE)
		u = (size_t)(emu->syscall_get_brk() - MMAP_BASE);

	if (emu->syscall_get_brk() - u >= emu->syscall_get_brk())
		goto err;

	RETURN(0);
	emu->syscall_set_brk(emu->syscall_get_brk()-u);
	return true;

err:
#endif
	RETURN(-EINVAL);
	return false;
}

bool arm_syscall_truncate(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ftruncate(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fchmod(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fchown(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getpriority(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setpriority(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_profil(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_statfs(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fstatfs(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ioperm(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_socketcall(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_syslog(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setitimer(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getitimer(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_mmap(armulator *emu)
{
	//if (PARM(3) != 0x22 /* MAP_PRIVATE|MAP_ANONYMOUS */) {
		RETURN(-EINVAL);
		return false;
	//}
#if 0
	/* check if overflow */
	if (emu->syscall_get_mmap_brk() + PARM(1) > emu->syscall_get_mmap_brk()) {
		RETURN(emu->syscall_get_mmap_brk());
		emu->syscall_set_mmap_brk(emu->syscall_get_mmap_brk() + PARM(1));
		return true;
	}
	else {
		RETURN(-EINVAL);
		return false;
	}
#endif
}


static void arm_swap_stat(struct stat *buf)
{
	arm_swap(&buf->st_dev, sizeof(&buf->st_dev));
	arm_swap(&buf->st_ino, sizeof(&buf->st_ino));
	arm_swap(&buf->st_mode, sizeof(&buf->st_mode));
	arm_swap(&buf->st_nlink, sizeof(&buf->st_nlink));
	arm_swap(&buf->st_uid, sizeof(&buf->st_uid));
	arm_swap(&buf->st_gid, sizeof(&buf->st_gid));
	arm_swap(&buf->st_rdev, sizeof(&buf->st_rdev));
	arm_swap(&buf->st_size, sizeof(&buf->st_size));
	arm_swap(&buf->st_blksize, sizeof(&buf->st_blksize));
	arm_swap(&buf->st_blocks, sizeof(&buf->st_blocks));
	arm_swap(&buf->st_atime, sizeof(&buf->st_atime));
	arm_swap(&buf->st_mtime, sizeof(&buf->st_mtime));
	arm_swap(&buf->st_ctime, sizeof(&buf->st_ctime));
	/*arm_swap(&buf->st_ino, sizeof(&buf->st_ino));*/
}

bool arm_syscall_stat(armulator *emu)
{

SYSCALL_STAGE_0
  {
	emu->syscall_open_addr = PARM(0);
	emu->syscall_open_len = 0;
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }

#ifdef COMPILE_SIMULATOR // emulator do not call twice
SYSCALL_STAGE_1
  {
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }
#endif

SYSCALL_STAGE_2
  {
	struct stat buf;
	target_addr_t buf_addr;

	buf_addr = PARM(1);
	if (emu->is_verbose())
		fprintf(stderr, "stat(file=\"%s\", buf=0x%08x)\n",
			(char *)(emu->syscall_buf), buf_addr);

	emu->syscall_iret = stat((char *)(emu->syscall_buf), &buf);
	if (emu->syscall_iret >= 0) {

		if (swap) arm_swap_stat(&buf);

		emu->syscall_alloc_buf(sizeof(struct stat));
		memcpy(emu->syscall_buf, &buf, sizeof(struct stat));

		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, sizeof(struct stat));
	}

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_3
  {
	SYSCALL_RESET_STAGE
	RETURN(emu->syscall_iret);
	return emu->syscall_iret != -1;
  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_lstat(armulator *emu)
{

SYSCALL_STAGE_0
  {
	emu->syscall_open_addr = PARM(0);
	emu->syscall_open_len = 0;
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }

#ifdef COMPILE_SIMULATOR // emulator do not call twice
SYSCALL_STAGE_1
  {
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }
#endif

SYSCALL_STAGE_2
  {
	struct stat buf;
	target_addr_t buf_addr;

	buf_addr = PARM(1);
	if (emu->is_verbose())
		fprintf(stderr, "lstat(file=\"%s\", buf=0x%08x)\n",
			(char *)(emu->syscall_buf), buf_addr);

	emu->syscall_iret = lstat((char *)(emu->syscall_buf), &buf);
	if (emu->syscall_iret >= 0) {
		if (swap) arm_swap_stat(&buf);
		emu->syscall_alloc_buf(sizeof(struct stat));
		memcpy(emu->syscall_buf, &buf, sizeof(struct stat));
		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, sizeof(struct stat));
	}

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_3
  {
	SYSCALL_RESET_STAGE
	RETURN(emu->syscall_iret);
	return emu->syscall_iret != -1;
  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_fstat(armulator *emu)
{
	
SYSCALL_STAGE_0
  {
	int fd;
	struct stat buf;
	target_addr_t buf_addr;

	fd = PARM(0);
	buf_addr = PARM(1);
	if (emu->is_verbose())
		fprintf(stderr, "fstat(fd=%d, buf=0x%08x)\n", fd, buf_addr);

	emu->syscall_iret = fstat(fd, &buf);
	if(emu->syscall_iret >= 0)
	{
		if(swap) arm_swap_stat(&buf);
		emu->syscall_alloc_buf(sizeof(struct stat));
		memcpy(emu->syscall_buf, &buf, sizeof(struct stat));
		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, sizeof(struct stat));
	}

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_1
  {
	SYSCALL_RESET_STAGE
	RETURN(emu->syscall_iret);
	return emu->syscall_iret != -1;
  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_olduname(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_iopl(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_vhangup(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_idle(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_vm86old(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_wait4(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_swapoff(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sysinfo(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ipc(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fsync(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sigreturn(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_clone(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setdomainname(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_uname(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_modify_ldt(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_adjtimex(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mprotect(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sigprocmask(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_create_module(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_init_module(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_delete_module(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_get_kernel_syms(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_quotactl(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getpgid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fchdir(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_bdflush(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sysfs(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_personality(armulator *emu) { return true; }

bool arm_syscall_afs_syscall(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setfsuid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setfsgid(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall__llseek(armulator *emu)
{

SYSCALL_STAGE_0
  {
	int fd;
	uint32_t offset_high;
	uint32_t offset_low;
	target_addr_t result_addr;
	int whence;

	fd = PARM(0);
	offset_high = PARM(1);
	offset_low = PARM(2);
	result_addr = PARM(3);
	whence = PARM(4);

	if (emu->is_verbose())
		fprintf(stderr, "_lseek(fd=%d, offset_high=%u, offset_low=%u,"
						" result=0x%08x, whence=%d)\n",
				fd, offset_high, offset_low, result_addr, whence);

	if(offset_high == 0)
	{
		off_t ret_off;
		ret_off = lseek(fd, offset_low, whence);

		if(ret_off >= 0)
		{
			loff_t ret_loff = ret_off;

			// how to swap this correctly remains a question
			if (swap) arm_swap(&ret_loff, sizeof(loff_t));
			emu->syscall_alloc_buf(sizeof(loff_t));
			memcpy(emu->syscall_buf, &ret_loff, sizeof(loff_t));
			MEM_WRITE_BLOCK(emu->syscall_buf, result_addr, sizeof(loff_t));
			SYSCALL_ADV_STAGE
		}
		else
		{
			SYSCALL_RESET_STAGE
			RETURN(-1);
			return false;
		}
	}
	else
	{
		SYSCALL_RESET_STAGE
		RETURN(-1);
		return false;
	}
  }

SYSCALL_STAGE_1
  {
	SYSCALL_RESET_STAGE
	RETURN(0);
	return true;
  }

SYSCALL_STAGE_FALSE

}

bool arm_syscall_getdents(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_newselect(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_flock(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_msync(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_readv(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_writev(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getsid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fdatasync(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sysctl(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mlock(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_munlock(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mlockall(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_munlockall(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sched_setparam(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sched_getparam(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sched_setscheduler(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getsheduler(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sched_yield(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sched_get_priority_max(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sched_get_priority_min(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sched_rr_get_interval(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_nanosleep(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mremap(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setresuid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getresuid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_vm86(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_query_module(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_poll(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_nfsservctl(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setresgid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getresgid(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_prctl(armulator *emu) { RETURN(-EINVAL); return false; }


bool arm_syscall_rt_sigreturn(armulator *emu) { RETURN(-EINVAL); return false;}
bool arm_syscall_rt_sigaction(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_rt_sigprocmask(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_rt_sigpending(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_rt_sigtimedwait(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_rt_sigqueueinfo(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_rt_sigsuspend(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_pread(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_pwrite(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_chown(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getcwd(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_capget(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_capset(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sigaltstack(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_sendfile(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getpmsg(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_putpmsg(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_vfork(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ugetrlimit(armulator *emu) { RETURN(-EINVAL); return false; }

bool arm_syscall_mmap2(armulator *emu)
{
	//if (PARM(3) != 0x22 /* MAP_PRIVATE|MAP_ANONYMOUS */) {
		RETURN(-EINVAL);
		return false;
	//}
	/* check if overflow */
	if (emu->syscall_get_mmap_brk() + PARM(1) > emu->syscall_get_mmap_brk()) {
		RETURN(emu->syscall_get_mmap_brk());
		emu->syscall_set_mmap_brk(emu->syscall_get_mmap_brk() + PARM(1));
		return true;
	}
	else {
		RETURN(-EINVAL);
		return false;
	}
}

bool arm_syscall_truncate64(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_ftruncate64(armulator *emu) { RETURN(-EINVAL); return false; }

#ifdef linux
static void arm_swap_stat64(struct stat64 *buf)
{
	arm_swap(&buf->st_dev, sizeof(&buf->st_dev));
	/*arm_swap(&buf->__pad1, sizeof(&buf->__pad1));*/
	arm_swap(&buf->st_ino, sizeof(&buf->st_ino));
	arm_swap(&buf->st_mode, sizeof(&buf->st_mode));
	arm_swap(&buf->st_nlink, sizeof(&buf->st_nlink));
	arm_swap(&buf->st_uid, sizeof(&buf->st_uid));
	arm_swap(&buf->st_gid, sizeof(&buf->st_gid));
	arm_swap(&buf->st_rdev, sizeof(&buf->st_rdev));
	/*arm_swap(&buf->__pad2, sizeof(&buf->__pad2));*/
	arm_swap(&buf->st_size, sizeof(&buf->st_size));
	arm_swap(&buf->st_blksize, sizeof(&buf->st_blksize));
	arm_swap(&buf->st_blocks, sizeof(&buf->st_blocks));
	arm_swap(&buf->st_atime, sizeof(&buf->st_atime));
	/*arm_swap(&buf->__unused1, sizeof(&buf->__unused1));*/
	arm_swap(&buf->st_mtime, sizeof(&buf->st_mtime));
	/*arm_swap(&buf->__unused2, sizeof(&buf->__unused2));*/
	arm_swap(&buf->st_ctime, sizeof(&buf->st_ctime));
	/*arm_swap(&buf->__unused3, sizeof(&buf->__unused3));*/
	/*arm_swap(&buf->st_ino, sizeof(&buf->st_ino));*/
}
#endif

bool arm_syscall_stat64(armulator *emu)
{

#ifdef linux
SYSCALL_STAGE_0
  {
	emu->syscall_open_addr = PARM(0);
	emu->syscall_open_len = 0;
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }

#ifdef COMPILE_SIMULATOR // emulator do not call twice
SYSCALL_STAGE_1
  {
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }
#endif

SYSCALL_STAGE_2
  {
	struct stat64 buf;
	target_addr_t buf_addr;

	buf_addr = PARM(1);
	if (emu->is_verbose())
		fprintf(stderr, "lstat(file=\"%s\", buf=0x%08x)\n",
			(char *)(emu->syscall_buf), buf_addr);

	emu->syscall_iret = lstat64((char *)(emu->syscall_buf), &buf);
	if (emu->syscall_iret >= 0) {
		if (swap) arm_swap_stat64(&buf);
		emu->syscall_alloc_buf(sizeof(struct stat64));
		memcpy(emu->syscall_buf, &buf, sizeof(struct stat64));
		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, sizeof(struct stat64));
	}

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_3
  {
	SYSCALL_RESET_STAGE
	RETURN(emu->syscall_iret);
	return emu->syscall_iret != -1;
  }

SYSCALL_STAGE_FALSE

#else
	RETURN(-1);
	return false;
#endif

}

bool arm_syscall_lstat64(armulator *emu)
{

#ifdef linux
SYSCALL_STAGE_0
  {
	emu->syscall_open_addr = PARM(0);
	emu->syscall_open_len = 0;
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }

#ifdef COMPILE_SIMULATOR // emulator do not call twice
SYSCALL_STAGE_1
  {
	arm_syscall_strdup(emu);
	SYSCALL_ENDOF_STAGE
  }
#endif

SYSCALL_STAGE_2
  {
	struct stat64 buf;
	target_addr_t buf_addr;

	buf_addr = PARM(1);
	if (emu->is_verbose())
		fprintf(stderr, "lstat(file=\"%s\", buf=0x%08x)\n",
			(char *)(emu->syscall_buf), buf_addr);

	emu->syscall_iret = lstat64((char *)(emu->syscall_buf), &buf);
	if (emu->syscall_iret >= 0) {
		if (swap) arm_swap_stat64(&buf);
		emu->syscall_alloc_buf(sizeof(struct stat64));
		memcpy(emu->syscall_buf, &buf, sizeof(struct stat64));
		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, sizeof(struct stat64));
	}

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_3
  {
	SYSCALL_RESET_STAGE
	RETURN(emu->syscall_iret);
	return emu->syscall_iret != -1;
  }

SYSCALL_STAGE_FALSE

#else
	RETURN(-1);
	return false;
#endif
}

bool arm_syscall_fstat64(armulator *emu)
{
#ifdef linux

SYSCALL_STAGE_0
  {
	int fd;
	struct stat64 buf;
	target_addr_t buf_addr;

	fd = PARM(0);
	buf_addr = PARM(1);
	if (emu->is_verbose())
		fprintf(stderr, "fstat(fd=%d, buf=0x%08x)\n", fd, buf_addr);

	emu->syscall_iret = fstat64(fd, &buf);
	if(emu->syscall_iret >= 0)
	{
		if(swap) arm_swap_stat64(&buf);
		emu->syscall_alloc_buf(sizeof(struct stat64));
		memcpy(emu->syscall_buf, &buf, sizeof(struct stat64));
		MEM_WRITE_BLOCK(emu->syscall_buf, buf_addr, sizeof(struct stat64));
	}

	SYSCALL_ADV_STAGE
  }

SYSCALL_STAGE_1
  {
	SYSCALL_RESET_STAGE
	RETURN(emu->syscall_iret);
	return emu->syscall_iret != -1;
  }

SYSCALL_STAGE_FALSE

#else
	RETURN(-1);
	return false;
#endif
}

bool arm_syscall_lchown32(armulator *emu) { RETURN(-EINVAL); return false; }

/*
 * We'll also let glibc handle checking
 * for 32-bit ids
 */
bool arm_syscall_getuid32(armulator *emu)
{
	uid_t uid;
	
	if (emu->is_verbose())
		fprintf(stderr, "getuid32()\n");
	uid = getuid();
	RETURN(uid);
	return true;
}

bool arm_syscall_getgid32(armulator *emu)
{
	gid_t gid;
	if (emu->is_verbose())
		fprintf(stderr, "getgid32()\n");
	gid = getgid();
	RETURN(gid);
	return true;
}

bool arm_syscall_geteuid32(armulator *emu)
{
	uid_t uid;
	
	if (emu->is_verbose())
		fprintf(stderr, "geteuid32()\n");
	uid = geteuid();
	RETURN(uid);
	return true;
}

bool arm_syscall_getegid32(armulator *emu)
{
	gid_t gid;
	
	if (emu->is_verbose())
		fprintf(stderr, "getegid32()\n");
	gid = getegid();
	RETURN(gid);
	return true;
}

bool arm_syscall_setreuid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setregid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getgroups32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setgroups32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fchown32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setresuid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getresuid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setresgid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getresgid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_chown32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setuid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setgid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setfsuid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_setfsgid32(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_pivot_root(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_mincore(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_madvise(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_getdents64(armulator *emu) { RETURN(-EINVAL); return false; }
bool arm_syscall_fcntl64(armulator *emu) { RETURN(-EINVAL); return false; }

static char *arm_get_syscall_name(int num)
{
	switch(num)
	{
		case __SYSCALL_exit: return "exit";
		case __SYSCALL_fork: return "fork";
		case __SYSCALL_read: return "read";
		case __SYSCALL_write: return "write";
		case __SYSCALL_open: return "open";
		case __SYSCALL_close: return "close";
		case __SYSCALL_waitpid: return "waitpid";
		case __SYSCALL_creat: return "creat";
		case __SYSCALL_link: return "link";
		case __SYSCALL_unlink: return "unlink";
		case __SYSCALL_execve: return "execve";
		case __SYSCALL_chdir: return "chdir";
		case __SYSCALL_time: return "time";
		case __SYSCALL_mknod: return "mknod";
		case __SYSCALL_chmod: return "chmod";
		case __SYSCALL_lchown: return "lchown";
		case __SYSCALL_break: return "break";
		case __SYSCALL_oldstat: return "oldstat";
		case __SYSCALL_lseek: return "lseek";
		case __SYSCALL_getpid: return "getpid";
		case __SYSCALL_mount: return "mount";
		case __SYSCALL_umount: return "umount";
		case __SYSCALL_setuid: return "setuid";
		case __SYSCALL_getuid: return "getuid";
		case __SYSCALL_stime: return "stime";
		case __SYSCALL_ptrace: return "ptrace";
		case __SYSCALL_alarm: return "alarm";
		case __SYSCALL_oldfstat: return "oldfstat";
		case __SYSCALL_pause: return "pause";
		case __SYSCALL_utime: return "utime";
		case __SYSCALL_stty: return "stty";
		case __SYSCALL_gtty: return "gtty";
		case __SYSCALL_access: return "access";
		case __SYSCALL_nice: return "nice";
		case __SYSCALL_ftime: return "ftime";
		case __SYSCALL_sync: return "sync";
		case __SYSCALL_kill: return "kill";
		case __SYSCALL_rename: return "rename";
		case __SYSCALL_mkdir: return "mkdir";
		case __SYSCALL_rmdir: return "rmdir";
		case __SYSCALL_dup: return "dup";
		case __SYSCALL_pipe: return "pipe";
		case __SYSCALL_times: return "times";
		case __SYSCALL_prof: return "prof";
		case __SYSCALL_brk: return "brk";
		case __SYSCALL_setgid: return "setgid";
		case __SYSCALL_getgid: return "getgid";
		case __SYSCALL_signal: return "signal";
		case __SYSCALL_geteuid: return "geteuid";
		case __SYSCALL_getegid: return "getegid";
		case __SYSCALL_acct: return "acct";
		case __SYSCALL_umount2: return "umount2";
		case __SYSCALL_lock: return "lock";
		case __SYSCALL_ioctl: return "ioctl";
		case __SYSCALL_fcntl: return "fcntl";
		case __SYSCALL_mpx: return "mpx";
		case __SYSCALL_setpgid: return "setpgid";
		case __SYSCALL_ulimit: return "ulimit";
		case __SYSCALL_oldolduname: return "oldolduname";
		case __SYSCALL_umask: return "umask";
		case __SYSCALL_chroot: return "chroot";
		case __SYSCALL_ustat	: return "ustat";
		case __SYSCALL_dup2: return "dup2";
		case __SYSCALL_getppid: return "getppid";
		case __SYSCALL_getpgrp: return "getpgrp";
		case __SYSCALL_setsid: return "setsid";
		case __SYSCALL_sigaction: return "sigaction";
		case __SYSCALL_sgetmask: return "sgetmask";
		case __SYSCALL_ssetmask: return "ssetmask";
		case __SYSCALL_setreuid: return "setreuid";
		case __SYSCALL_setregid: return "setregid";
		case __SYSCALL_sigsuspend: return "sigsuspend";
		case __SYSCALL_sigpending: return "sigpending";
		case __SYSCALL_sethostname: return "sethostname";
		case __SYSCALL_setrlimit: return "setrlimit";
		case __SYSCALL_getrlimit: return "getrlimit";
		case __SYSCALL_getrusage: return "getrusage";
		case __SYSCALL_gettimeofday: return "gettimeofday";
		case __SYSCALL_settimeofday: return "settimeofday";
		case __SYSCALL_getgroups: return "getgroups";
		case __SYSCALL_setgroups: return "setgroups";
		case __SYSCALL_select: return "select";
		case __SYSCALL_symlink: return "symlink";
		case __SYSCALL_oldlstat: return "oldlstat";
		case __SYSCALL_readlink: return "readlink";
		case __SYSCALL_uselib: return "uselib";
		case __SYSCALL_swapon: return "swapon";
		case __SYSCALL_reboot: return "reboot";
		case __SYSCALL_readdir: return "readdir";
		case __SYSCALL_mmap: return "mmap";
		case __SYSCALL_munmap: return "munmap";
		case __SYSCALL_truncate: return "truncate";
		case __SYSCALL_ftruncate: return "ftruncate";
		case __SYSCALL_fchmod: return "fchmod";
		case __SYSCALL_fchown: return "fchown";
		case __SYSCALL_getpriority: return "getpriority";
		case __SYSCALL_setpriority: return "setpriority";
		case __SYSCALL_profil: return "profil";
		case __SYSCALL_statfs: return "statfs";
		case __SYSCALL_fstatfs: return "fstatfs";
		case __SYSCALL_ioperm: return "ioperm";
		case __SYSCALL_socketcall: return "socketcall";
		case __SYSCALL_syslog: return "syslog";
		case __SYSCALL_setitimer: return "setitimer";
		case __SYSCALL_getitimer: return "getitimer";
		case __SYSCALL_stat: return "stat";
		case __SYSCALL_lstat: return "lstat";
		case __SYSCALL_fstat: return "fstat";
		case __SYSCALL_olduname: return "olduname";
		case __SYSCALL_iopl: return "iopl";
		case __SYSCALL_vhangup: return "vhangup";
		case __SYSCALL_idle: return "idle";
		case __SYSCALL_vm86old: return "vm86old";
		case __SYSCALL_wait4: return "wait4";
		case __SYSCALL_swapoff: return "swapoff";
		case __SYSCALL_sysinfo: return "sysinfo";
		case __SYSCALL_ipc: return "ipc";
		case __SYSCALL_fsync: return "fsync";
		case __SYSCALL_sigreturn: return "sigreturn";
		case __SYSCALL_clone: return "clone";
		case __SYSCALL_setdomainname: return "setdomainname";
		case __SYSCALL_uname: return "uname";
		case __SYSCALL_modify_ldt: return "modify_ldt";
		case __SYSCALL_adjtimex: return "adjtimex";
		case __SYSCALL_mprotect: return "mprotect";
		case __SYSCALL_sigprocmask: return "sigprocmask";
		case __SYSCALL_create_module: return "create_module";
		case __SYSCALL_init_module: return "init_module";
		case __SYSCALL_delete_module: return "delete_module";
		case __SYSCALL_get_kernel_syms: return "get_kernel_syms";
		case __SYSCALL_quotactl: return "quotactl";
		case __SYSCALL_getpgid: return "getpgid";
		case __SYSCALL_fchdir: return "fchdir";
		case __SYSCALL_bdflush: return "bdflush";
		case __SYSCALL_sysfs: return "sysfs";
		case __SYSCALL_personality: return "personality";
		case __SYSCALL_afs_syscall: return "afs_syscall";
		case __SYSCALL_setfsuid: return "setfsuid";
		case __SYSCALL_setfsgid: return "setfsgid";
		case __SYSCALL__llseek: return "_llseek";
		case __SYSCALL_getdents: return "getdents";
		case __SYSCALL__newselect: return "newselect";
		case __SYSCALL_flock: return "flock";
		case __SYSCALL_msync: return "msync";
		case __SYSCALL_readv: return "readv";
		case __SYSCALL_writev: return "writev";
		case __SYSCALL_getsid: return "getsid";
		case __SYSCALL_fdatasync: return "fdatasync";
		case __SYSCALL__sysctl: return "sysctl";
		case __SYSCALL_mlock: return "mlock";
		case __SYSCALL_munlock: return "munlock";
		case __SYSCALL_mlockall: return "mlockall";
		case __SYSCALL_munlockall: return "munlockall";
		case __SYSCALL_sched_setparam: return "sched_setparam";
		case __SYSCALL_sched_getparam: return "sched_getparam";
		case __SYSCALL_sched_setscheduler: return "sched_setscheduler";
		case __SYSCALL_sched_getscheduler: return "getsheduler";
		case __SYSCALL_sched_yield: return "sched_yield";
		case __SYSCALL_sched_get_priority_max: return "sched_get_priority_max";
		case __SYSCALL_sched_get_priority_min: return "sched_get_priority_min";
		case __SYSCALL_sched_rr_get_interval: return "sched_rr_get_interval";
		case __SYSCALL_nanosleep: return "nanosleep";
		case __SYSCALL_mremap: return "mremap";
		case __SYSCALL_setresuid	: return "setresuid";
		case __SYSCALL_getresuid: return "getresuid";
		case __SYSCALL_vm86: return "vm86";
		case __SYSCALL_query_module: return "query_module";
		case __SYSCALL_poll: return "poll";
		case __SYSCALL_nfsservctl: return "nfsservctl";
		case __SYSCALL_setresgid	: return "setresgid";
		case __SYSCALL_getresgid: return "getresgid";
		case __SYSCALL_prctl: return "prctl";
		case __SYSCALL_rt_sigreturn: return "rt_sigreturn";
		case __SYSCALL_rt_sigaction: return "rt_sigaction";
		case __SYSCALL_rt_sigprocmask: return "rt_sigprocmask";
		case __SYSCALL_rt_sigpending: return "rt_sigpending";
		case __SYSCALL_rt_sigtimedwait: return "rt_sigtimedwait";
		case __SYSCALL_rt_sigqueueinfo: return "rt_sigqueueinfo";
		case __SYSCALL_rt_sigsuspend: return "rt_sigsuspend";
		case __SYSCALL_pread: return "pread";
		case __SYSCALL_pwrite: return "pwrite";
		case __SYSCALL_chown: return "chown";
		case __SYSCALL_getcwd: return "getcwd";
		case __SYSCALL_capget: return "capget";
		case __SYSCALL_capset: return "capset";
		case __SYSCALL_sigaltstack: return "sigaltstack";
		case __SYSCALL_sendfile: return "sendfile";
		case __SYSCALL_getpmsg: return "getpmsg";
		case __SYSCALL_putpmsg: return "putpmsg";
		case __SYSCALL_vfork: return "vfork";
		case __SYSCALL_ugetrlimit: return "ugetrlimit";
		case __SYSCALL_mmap2: return "mmap2";
		case __SYSCALL_truncate64: return "truncate64";
		case __SYSCALL_ftruncate64: return "ftruncate64";
		case __SYSCALL_stat64: return "stat64";
		case __SYSCALL_lstat64: return "lstat64";
		case __SYSCALL_fstat64: return "fstat64";
		case __SYSCALL_lchown32: return "lchown32";
		case __SYSCALL_getuid32: return "getuid32";
		case __SYSCALL_getgid32: return "getgid32";
		case __SYSCALL_geteuid32: return "geteuid32";
		case __SYSCALL_getegid32: return "getegid32";
		case __SYSCALL_setreuid32: return "setreuid32";
		case __SYSCALL_setregid32: return "setregid32";
		case __SYSCALL_getgroups32: return "getgroups32";
		case __SYSCALL_setgroups32: return "setgroups32";
		case __SYSCALL_fchown32: return "fchown32";
		case __SYSCALL_setresuid32: return "setresuid32";
		case __SYSCALL_getresuid32: return "getresuid32";
		case __SYSCALL_setresgid32: return "setresgid32";
		case __SYSCALL_getresgid32: return "getresgid32";
		case __SYSCALL_chown32: return "chown32";
		case __SYSCALL_setuid32: return "setuid32";
		case __SYSCALL_setgid32: return "setgid32";
		case __SYSCALL_setfsuid32: return "setfsuid32";
		case __SYSCALL_setfsgid32: return "setfsgid32";
		case __SYSCALL_pivot_root: return "pivot_root";
		case __SYSCALL_mincore: return "mincore";
		case __SYSCALL_madvise: return "madvise";
		case __SYSCALL_getdents64: return "getdents64";
		case __SYSCALL_fcntl64: return "fcntl64";
	}
	return "?";
}

void do_syscall(armulator *emu, arm_inst_t inst)
{
	int syscall_num;
	bool ret = false;
	
	syscall_num	= SYSCALL_NUM & 0xff;
	if (emu->is_verbose())
		fprintf(stderr, "got a system call (number : %u, name : %s)\n", syscall_num, arm_get_syscall_name(syscall_num));
		
	fflush(stderr);
	switch(syscall_num)
	{
		case __SYSCALL_exit: ret = arm_syscall_exit(emu); break;
		case __SYSCALL_fork: ret = arm_syscall_fork(emu); break;
		case __SYSCALL_read: ret = arm_syscall_read(emu); break;
		case __SYSCALL_write: ret = arm_syscall_write(emu); break;
		case __SYSCALL_open: ret = arm_syscall_open(emu); break;
		case __SYSCALL_close: ret = arm_syscall_close(emu); break;
		case __SYSCALL_waitpid: ret = arm_syscall_waitpid(emu); break;
		case __SYSCALL_creat: ret = arm_syscall_creat(emu); break;
		case __SYSCALL_link: ret = arm_syscall_link(emu); break;
		case __SYSCALL_unlink: ret = arm_syscall_unlink(emu); break;
		case __SYSCALL_execve: ret = arm_syscall_execve(emu); break;
		case __SYSCALL_chdir: ret = arm_syscall_chdir(emu); break;
		case __SYSCALL_time: ret = arm_syscall_time(emu); break;
		case __SYSCALL_mknod: ret = arm_syscall_mknod(emu); break;
		case __SYSCALL_chmod: ret = arm_syscall_chmod(emu); break;
		case __SYSCALL_lchown: ret = arm_syscall_lchown(emu); break;
		case __SYSCALL_break: ret = arm_syscall_break(emu); break;
		case __SYSCALL_oldstat: ret = arm_syscall_oldstat(emu); break;
		case __SYSCALL_lseek: ret = arm_syscall_lseek(emu); break;
		case __SYSCALL_getpid: ret = arm_syscall_getpid(emu); break;
		case __SYSCALL_mount: ret = arm_syscall_mount(emu); break;
		case __SYSCALL_umount: ret = arm_syscall_umount(emu); break;
		case __SYSCALL_setuid: ret = arm_syscall_setuid(emu); break;
		case __SYSCALL_getuid: ret = arm_syscall_getuid(emu); break;
		case __SYSCALL_stime: ret = arm_syscall_stime(emu); break;
		case __SYSCALL_ptrace: ret = arm_syscall_ptrace(emu); break;
		case __SYSCALL_alarm: ret = arm_syscall_alarm(emu); break;
		case __SYSCALL_oldfstat: ret = arm_syscall_oldfstat(emu); break;
		case __SYSCALL_pause: ret = arm_syscall_pause(emu); break;
		case __SYSCALL_utime: ret = arm_syscall_utime(emu); break;
		case __SYSCALL_stty: ret = arm_syscall_stty(emu); break;
		case __SYSCALL_gtty: ret = arm_syscall_gtty(emu); break;
		case __SYSCALL_access: ret = arm_syscall_access(emu); break;
		case __SYSCALL_nice: ret = arm_syscall_nice(emu); break;
		case __SYSCALL_ftime: ret = arm_syscall_ftime(emu); break;
		case __SYSCALL_sync: ret = arm_syscall_sync(emu); break;
		case __SYSCALL_kill: ret = arm_syscall_kill(emu); break;
		case __SYSCALL_rename: ret = arm_syscall_rename(emu); break;
		case __SYSCALL_mkdir: ret = arm_syscall_mkdir(emu); break;
		case __SYSCALL_rmdir: ret = arm_syscall_rmdir(emu); break;
		case __SYSCALL_dup: ret = arm_syscall_dup(emu); break;
		case __SYSCALL_pipe: ret = arm_syscall_pipe(emu); break;
		case __SYSCALL_times: ret = arm_syscall_times(emu); break;
		case __SYSCALL_prof: ret = arm_syscall_prof(emu); break;
		case __SYSCALL_brk: ret = arm_syscall_brk(emu); break;
		case __SYSCALL_setgid: ret = arm_syscall_setgid(emu); break;
		case __SYSCALL_getgid: ret = arm_syscall_getgid(emu); break;
		case __SYSCALL_signal: ret = arm_syscall_signal(emu); break;
		case __SYSCALL_geteuid: ret = arm_syscall_geteuid(emu); break;
		case __SYSCALL_getegid: ret = arm_syscall_getegid(emu); break;
		case __SYSCALL_acct: ret = arm_syscall_acct(emu); break;
		case __SYSCALL_umount2: ret = arm_syscall_umount2(emu); break;
		case __SYSCALL_lock: ret = arm_syscall_lock(emu); break;
		case __SYSCALL_ioctl: ret = arm_syscall_ioctl(emu); break;
		case __SYSCALL_fcntl: ret = arm_syscall_fcntl(emu); break;
		case __SYSCALL_mpx: ret = arm_syscall_mpx(emu); break;
		case __SYSCALL_setpgid: ret = arm_syscall_setpgid(emu); break;
		case __SYSCALL_ulimit: ret = arm_syscall_ulimit(emu); break;
		case __SYSCALL_oldolduname: ret = arm_syscall_oldolduname(emu); break;
		case __SYSCALL_umask: ret = arm_syscall_umask(emu); break;
		case __SYSCALL_chroot: ret = arm_syscall_chroot(emu); break;
		case __SYSCALL_ustat	: ret = arm_syscall_ustat(emu); break;
		case __SYSCALL_dup2: ret = arm_syscall_dup2(emu); break;
		case __SYSCALL_getppid: ret = arm_syscall_getppid(emu); break;
		case __SYSCALL_getpgrp: ret = arm_syscall_getpgrp(emu); break;
		case __SYSCALL_setsid: ret = arm_syscall_setsid(emu); break;
		case __SYSCALL_sigaction: ret = arm_syscall_sigaction(emu); break;
		case __SYSCALL_sgetmask: ret = arm_syscall_sgetmask(emu); break;
		case __SYSCALL_ssetmask: ret = arm_syscall_ssetmask(emu); break;
		case __SYSCALL_setreuid: ret = arm_syscall_setreuid(emu); break;
		case __SYSCALL_setregid: ret = arm_syscall_setregid(emu); break;
		case __SYSCALL_sigsuspend: ret = arm_syscall_sigsuspend(emu); break;
		case __SYSCALL_sigpending: ret = arm_syscall_sigpending(emu); break;
		case __SYSCALL_sethostname: ret = arm_syscall_sethostname(emu); break;
		case __SYSCALL_setrlimit: ret = arm_syscall_setrlimit(emu); break;
		case __SYSCALL_getrlimit: ret = arm_syscall_getrlimit(emu); break;
		case __SYSCALL_getrusage: ret = arm_syscall_getrusage(emu); break;
		case __SYSCALL_gettimeofday: ret = arm_syscall_gettimeofday(emu); break;
		case __SYSCALL_settimeofday: ret = arm_syscall_settimeofday(emu); break;
		case __SYSCALL_getgroups: ret = arm_syscall_getgroups(emu); break;
		case __SYSCALL_setgroups: ret = arm_syscall_setgroups(emu); break;
		case __SYSCALL_select: ret = arm_syscall_select(emu); break;
		case __SYSCALL_symlink: ret = arm_syscall_symlink(emu); break;
		case __SYSCALL_oldlstat: ret = arm_syscall_oldlstat(emu); break;
		case __SYSCALL_readlink: ret = arm_syscall_readlink(emu); break;
		case __SYSCALL_uselib: ret = arm_syscall_uselib(emu); break;
		case __SYSCALL_swapon: ret = arm_syscall_swapon(emu); break;
		case __SYSCALL_reboot: ret = arm_syscall_reboot(emu); break;
		case __SYSCALL_readdir: ret = arm_syscall_readdir(emu); break;
		case __SYSCALL_mmap: ret = arm_syscall_mmap(emu); break;
		case __SYSCALL_munmap: ret = arm_syscall_munmap(emu); break;
		case __SYSCALL_truncate: ret = arm_syscall_truncate(emu); break;
		case __SYSCALL_ftruncate: ret = arm_syscall_ftruncate(emu); break;
		case __SYSCALL_fchmod: ret = arm_syscall_fchmod(emu); break;
		case __SYSCALL_fchown: ret = arm_syscall_fchown(emu); break;
		case __SYSCALL_getpriority: ret = arm_syscall_getpriority(emu); break;
		case __SYSCALL_setpriority: ret = arm_syscall_setpriority(emu); break;
		case __SYSCALL_profil: ret = arm_syscall_profil(emu); break;
		case __SYSCALL_statfs: ret = arm_syscall_statfs(emu); break;
		case __SYSCALL_fstatfs: ret = arm_syscall_fstatfs(emu); break;
		case __SYSCALL_ioperm: ret = arm_syscall_ioperm(emu); break;
		case __SYSCALL_socketcall: ret = arm_syscall_socketcall(emu); break;
		case __SYSCALL_syslog: ret = arm_syscall_syslog(emu); break;
		case __SYSCALL_setitimer: ret = arm_syscall_setitimer(emu); break;
		case __SYSCALL_getitimer: ret = arm_syscall_getitimer(emu); break;
		case __SYSCALL_stat: ret = arm_syscall_stat(emu); break;
		case __SYSCALL_lstat: ret = arm_syscall_lstat(emu); break;
		case __SYSCALL_fstat: ret = arm_syscall_fstat(emu); break;
		case __SYSCALL_olduname: ret = arm_syscall_olduname(emu); break;
		case __SYSCALL_iopl: ret = arm_syscall_iopl(emu); break;
		case __SYSCALL_vhangup: ret = arm_syscall_vhangup(emu); break;
		case __SYSCALL_idle: ret = arm_syscall_idle(emu); break;
		case __SYSCALL_vm86old: ret = arm_syscall_vm86old(emu); break;
		case __SYSCALL_wait4: ret = arm_syscall_wait4(emu); break;
		case __SYSCALL_swapoff: ret = arm_syscall_swapoff(emu); break;
		case __SYSCALL_sysinfo: ret = arm_syscall_sysinfo(emu); break;
		case __SYSCALL_ipc: ret = arm_syscall_ipc(emu); break;
		case __SYSCALL_fsync: ret = arm_syscall_fsync(emu); break;
		case __SYSCALL_sigreturn: ret = arm_syscall_sigreturn(emu); break;
		case __SYSCALL_clone: ret = arm_syscall_clone(emu); break;
		case __SYSCALL_setdomainname: ret = arm_syscall_setdomainname(emu); break;
		case __SYSCALL_uname: ret = arm_syscall_uname(emu); break;
		case __SYSCALL_modify_ldt: ret = arm_syscall_modify_ldt(emu); break;
		case __SYSCALL_adjtimex: ret = arm_syscall_adjtimex(emu); break;
		case __SYSCALL_mprotect: ret = arm_syscall_mprotect(emu); break;
		case __SYSCALL_sigprocmask: ret = arm_syscall_sigprocmask(emu); break;
		case __SYSCALL_create_module: ret = arm_syscall_create_module(emu); break;
		case __SYSCALL_init_module: ret = arm_syscall_init_module(emu); break;
		case __SYSCALL_delete_module: ret = arm_syscall_delete_module(emu); break;
		case __SYSCALL_get_kernel_syms: ret = arm_syscall_get_kernel_syms(emu); break;
		case __SYSCALL_quotactl: ret = arm_syscall_quotactl(emu); break;
		case __SYSCALL_getpgid: ret = arm_syscall_getpgid(emu); break;
		case __SYSCALL_fchdir: ret = arm_syscall_fchdir(emu); break;
		case __SYSCALL_bdflush: ret = arm_syscall_bdflush(emu); break;
		case __SYSCALL_sysfs: ret = arm_syscall_sysfs(emu); break;
		case __SYSCALL_personality: ret = arm_syscall_personality(emu); break;
		case __SYSCALL_afs_syscall: ret = arm_syscall_afs_syscall(emu); break;
		case __SYSCALL_setfsuid: ret = arm_syscall_setfsuid(emu); break;
		case __SYSCALL_setfsgid: ret = arm_syscall_setfsgid(emu); break;
		case __SYSCALL__llseek: ret = arm_syscall__llseek(emu); break;
		case __SYSCALL_getdents: ret = arm_syscall_getdents(emu); break;
		case __SYSCALL__newselect: ret = arm_syscall_newselect(emu); break;
		case __SYSCALL_flock: ret = arm_syscall_flock(emu); break;
		case __SYSCALL_msync: ret = arm_syscall_msync(emu); break;
		case __SYSCALL_readv: ret = arm_syscall_readv(emu); break;
		case __SYSCALL_writev: ret = arm_syscall_writev(emu); break;
		case __SYSCALL_getsid: ret = arm_syscall_getsid(emu); break;
		case __SYSCALL_fdatasync: ret = arm_syscall_fdatasync(emu); break;
		case __SYSCALL__sysctl: ret = arm_syscall_sysctl(emu); break;
		case __SYSCALL_mlock: ret = arm_syscall_mlock(emu); break;
		case __SYSCALL_munlock: ret = arm_syscall_munlock(emu); break;
		case __SYSCALL_mlockall: ret = arm_syscall_mlockall(emu); break;
		case __SYSCALL_munlockall: ret = arm_syscall_munlockall(emu); break;
		case __SYSCALL_sched_setparam: ret = arm_syscall_sched_setparam(emu); break;
		case __SYSCALL_sched_getparam: ret = arm_syscall_sched_getparam(emu); break;
		case __SYSCALL_sched_setscheduler: ret = arm_syscall_sched_setscheduler(emu); break;
		case __SYSCALL_sched_getscheduler: ret = arm_syscall_getsheduler(emu); break;
		case __SYSCALL_sched_yield: ret = arm_syscall_sched_yield(emu); break;
		case __SYSCALL_sched_get_priority_max: ret = arm_syscall_sched_get_priority_max(emu); break;
		case __SYSCALL_sched_get_priority_min: ret = arm_syscall_sched_get_priority_min(emu); break;
		case __SYSCALL_sched_rr_get_interval: ret = arm_syscall_sched_rr_get_interval(emu); break;
		case __SYSCALL_nanosleep: ret = arm_syscall_nanosleep(emu); break;
		case __SYSCALL_mremap: ret = arm_syscall_mremap(emu); break;
		case __SYSCALL_setresuid	: ret = arm_syscall_setresuid(emu); break;
		case __SYSCALL_getresuid: ret = arm_syscall_getresuid(emu); break;
		case __SYSCALL_vm86: ret = arm_syscall_vm86(emu); break;
		case __SYSCALL_query_module: ret = arm_syscall_query_module(emu); break;
		case __SYSCALL_poll: ret = arm_syscall_poll(emu); break;
		case __SYSCALL_nfsservctl: ret = arm_syscall_nfsservctl(emu); break;
		case __SYSCALL_setresgid	: ret = arm_syscall_setresgid(emu); break;
		case __SYSCALL_getresgid: ret = arm_syscall_getresgid(emu); break;
		case __SYSCALL_prctl: ret = arm_syscall_prctl(emu); break;
		case __SYSCALL_rt_sigreturn: ret = arm_syscall_rt_sigreturn(emu); break;
		case __SYSCALL_rt_sigaction: ret = arm_syscall_rt_sigaction(emu); break;
		case __SYSCALL_rt_sigprocmask: ret = arm_syscall_rt_sigprocmask(emu); break;
		case __SYSCALL_rt_sigpending: ret = arm_syscall_rt_sigpending(emu); break;
		case __SYSCALL_rt_sigtimedwait: ret = arm_syscall_rt_sigtimedwait(emu); break;
		case __SYSCALL_rt_sigqueueinfo: ret = arm_syscall_rt_sigqueueinfo(emu); break;
		case __SYSCALL_rt_sigsuspend: ret = arm_syscall_rt_sigsuspend(emu); break;
		case __SYSCALL_pread: ret = arm_syscall_pread(emu); break;
		case __SYSCALL_pwrite: ret = arm_syscall_pwrite(emu); break;
		case __SYSCALL_chown: ret = arm_syscall_chown(emu); break;
		case __SYSCALL_getcwd: ret = arm_syscall_getcwd(emu); break;
		case __SYSCALL_capget: ret = arm_syscall_capget(emu); break;
		case __SYSCALL_capset: ret = arm_syscall_capset(emu); break;
		case __SYSCALL_sigaltstack: ret = arm_syscall_sigaltstack(emu); break;
		case __SYSCALL_sendfile: ret = arm_syscall_sendfile(emu); break;
		case __SYSCALL_getpmsg: ret = arm_syscall_getpmsg(emu); break;
		case __SYSCALL_putpmsg: ret = arm_syscall_putpmsg(emu); break;
		case __SYSCALL_vfork: ret = arm_syscall_vfork(emu); break;
		case __SYSCALL_ugetrlimit: ret = arm_syscall_ugetrlimit(emu); break;
		case __SYSCALL_mmap2: ret = arm_syscall_mmap2(emu); break;
		case __SYSCALL_truncate64: ret = arm_syscall_truncate64(emu); break;
		case __SYSCALL_ftruncate64: ret = arm_syscall_ftruncate64(emu); break;
		case __SYSCALL_stat64: ret = arm_syscall_stat64(emu); break;
		case __SYSCALL_lstat64: ret = arm_syscall_lstat64(emu); break;
		case __SYSCALL_fstat64: ret = arm_syscall_fstat64(emu); break;
		case __SYSCALL_lchown32: ret = arm_syscall_lchown32(emu); break;
		case __SYSCALL_getuid32: ret = arm_syscall_getuid32(emu); break;
		case __SYSCALL_getgid32: ret = arm_syscall_getgid32(emu); break;
		case __SYSCALL_geteuid32: ret = arm_syscall_geteuid32(emu); break;
		case __SYSCALL_getegid32: ret = arm_syscall_getegid32(emu); break;
		case __SYSCALL_setreuid32: ret = arm_syscall_setreuid32(emu); break;
		case __SYSCALL_setregid32: ret = arm_syscall_setregid32(emu); break;
		case __SYSCALL_getgroups32: ret = arm_syscall_getgroups32(emu); break;
		case __SYSCALL_setgroups32: ret = arm_syscall_setgroups32(emu); break;
		case __SYSCALL_fchown32: ret = arm_syscall_fchown32(emu); break;
		case __SYSCALL_setresuid32: ret = arm_syscall_setresuid32(emu); break;
		case __SYSCALL_getresuid32: ret = arm_syscall_getresuid32(emu); break;
		case __SYSCALL_setresgid32: ret = arm_syscall_setresgid32(emu); break;
		case __SYSCALL_getresgid32: ret = arm_syscall_getresgid32(emu); break;
		case __SYSCALL_chown32: ret = arm_syscall_chown32(emu); break;
		case __SYSCALL_setuid32: ret = arm_syscall_setuid32(emu); break;
		case __SYSCALL_setgid32: ret = arm_syscall_setgid32(emu); break;
		case __SYSCALL_setfsuid32: ret = arm_syscall_setfsuid32(emu); break;
		case __SYSCALL_setfsgid32: ret = arm_syscall_setfsgid32(emu); break;
		case __SYSCALL_pivot_root: ret = arm_syscall_pivot_root(emu); break;
		case __SYSCALL_mincore: ret = arm_syscall_mincore(emu); break;
		case __SYSCALL_madvise: ret = arm_syscall_madvise(emu); break;
		case __SYSCALL_getdents64: ret = arm_syscall_getdents64(emu); break;
		case __SYSCALL_fcntl64: ret = arm_syscall_fcntl64(emu); break;
	}
	
	if(!ret)
	{
		if (emu->is_verbose())
			fprintf(stderr, "Warning : system call returns an error (number : %u, name : %s)\n", syscall_num, arm_get_syscall_name(syscall_num));
	}
	else
	{
	}
}

