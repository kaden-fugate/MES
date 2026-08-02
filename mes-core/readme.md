# Business logic code (C++)

# DB POOL:
The database pool class is in charge of initializing several readers/writers
at a time and dispatching properly.

No two transactions should be using the same connection to the database.
Currently, with SQLite, we can only allocate 1 reader at a time. Though before
we start intense simulations, this number should suffice.

db_pool is derived from the r_pool detailed below. db_pool is allows writer
connection to the database. this is due to the execute function implemented
in this class.

# READ POOL:
r_pool is in charge of the readers. We can allocate many read-only connections.
The trick is that SQLLite doesn't actually have a "read-only" connection so we
must just limit the capability to read only in this class.

Several readers can be allocated allowing us to run multiple read-only 
transactions at a time to speed up our systems.

# TRANSACTIONS (TX):

#   Base class:
    Each transaction derives from an abstract transaction base class.
    From there, a transcation will implement its own child class and define
    the exec function which will determine what operations the transaction
    will complete.

    Furthermore, each child transaction will have to implement its own input
    and output types.

#   Transaction list:
#       000-099 (Inquiry TX's):
        000: Query lot info
        ... TBD ...

#       100-299 (Update TX's):
        ... TBD ...

#       300-XXX (TDB):
