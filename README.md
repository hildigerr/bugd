# BugD
Copyright (c) 2013 Roberto Morrel HildigerR Vergaray

I started writing this graphical bug tracker in order to learn GTK and the C API for SQLite.
I'm calling it bugd for "bug database." I plan to use it with my school assignments.
It is available to be installed via my ppa <https://launchpad.net/~moonsdad/+archive/ppa>

So far, you can open/create a database, load it with bug reports, change their status
between 0 and 1, and view/edit the bug's data. See "todo.txt" for unimplemented feature plans.

Known issues:
<ul>
    <li>Clicking "Add", with an open database, twice will open two bug report windows which secretly share the same buffer.</li>
</ul>

Bug Reports in the database consist of:
<ul>
    <li> Id INTEGER PRIMARY KEY,</li>
    <li> Name TEXT,          -- This must be unique for each bug reported (for now)</li>
    <li> Reproduce TEXT,     -- Provide instructions for reproducing the issue.</li>
    <li> Expectation TEXT,   -- This is where you put a description of what you expect to be happening.</li>
    <li> Behavior TEXT,      -- Describe what is actually happening.</li>
    <li> Notes TEXT,         -- This is for anything else (Eg: Who is assigned to work on this problem).</li>
    <li> Status INTEGER      -- 0 == opened ; 1 == solved</li>
</ul>

I originally used the icon from from:
http://findicons.com/icon/203378/bug_buddy?id=204799
But, now I'm using an icon that I made. A bit of trivia about it is that it contains a secret message. I'm sure someone will find it easily if they try. Please, let me know if you do.
