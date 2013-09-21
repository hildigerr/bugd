To learn GTK and the C API for SQLite, I started writing this graphical bug tracker.
I'm calling it bugd for "bug database." I plan to use it with my school assignments.

So far you can open/create a database, load it with bug reports, and change their status
between 0 and 1.

Bugs in the database consist of:
    * Id INTEGER PRIMARY KEY,
    * Name TEXT,          -- This must be unique for each bug reported (for now)
    * Reproduce TEXT,     -- Provide instructions for reproducing the issue.
    * Expectation TEXT,   -- This is where you put a description of what you expect to be happening.
    * Behavior TEXT,      -- Describe what is actually happening.
    * Notes TEXT,         -- This is for anything else (Eg: Who is assigned to work on this problem).
    * Status INTEGER      -- 0 == opened ; 1 == solved
