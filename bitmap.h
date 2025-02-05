/*
 * bitmap.h: Copyright (C) Peter T. Breuer (ptb@ot.uc3m.es) 2003
 *
 * additions: Copyright (C) 2003-2004, Paul Clements, SteelEye Technology, Inc.
 */
#ifndef BITMAP_H
#define BITMAP_H

#define BITMAP_MAJOR_LO 3
/* version 4 insists the bitmap is in little-endian order
 * with version 3, it is host-endian which is non-portable
 */
#define BITMAP_MAJOR_HI 4
#define	BITMAP_MAJOR_HOSTENDIAN 3
#define	BITMAP_MAJOR_CLUSTERED 5

#define BITMAP_MAGIC 0x6d746962

/* use these for bitmap->flags and bitmap->sb->state bit-fields */
enum bitmap_state {
	BITMAP_ACTIVE = 0x001, /* the bitmap is in use */
	BITMAP_STALE  = 0x002  /* the bitmap file is out of date or had -EIO */
};

/* the superblock at the front of the bitmap file -- little endian */
typedef struct bitmap_super_s {
	__u32 magic;        /*  0  BITMAP_MAGIC */
	__u32 version;      /*  4  the bitmap major for now, could change... */
	__u8  uuid[16];     /*  8  128 bit uuid - must match md device uuid */
	__u64 events;       /* 24  event counter for the bitmap (1)*/
	__u64 events_cleared;/*32  event counter when last bit cleared (2) */
	__u64 sync_size;    /* 40  the size of the md device's sync range(3) */
	__u32 state;        /* 48  bitmap state information */
	__u32 chunksize;    /* 52  the bitmap chunk size in bytes */
	__u32 daemon_sleep; /* 56  seconds between disk flushes */
	__u32 write_behind; /* 60  number of outstanding write-behind writes */
	__u32 sectors_reserved; /* 64 number of 512-byte sectors that are
				 * reserved for the bitmap. */
	__u32 nodes;        /* 68 the maximum number of nodes in cluster. */
	__u8 cluster_name[64]; /* 72 cluster name to which this md belongs */
	__u8  pad[256 - 136]; /* set to zero */
} bitmap_super_t;

/* notes:
 * (1) This event counter is updated before the eventcounter in the md superblock
 *    When a bitmap is loaded, it is only accepted if this event counter is equal
 *    to, or one greater than, the event counter in the superblock.
 * (2) This event counter is updated when the other one is *if*and*only*if* the
 *    array is not degraded.  As bits are not cleared when the array is degraded,
 *    this represents the last time that any bits were cleared.
 *    If a device is being added that has an event count with this value or
 *    higher, it is accepted as conforming to the bitmap.
 * (3)This is the number of sectors represented by the bitmap, and is the range that
 *    resync happens across.  For raid1 and raid5/6 it is the size of individual
 *    devices.  For raid10 it is the size of the array.
 */

#endif
