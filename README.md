### Goals
 - What problem am I solving?
	 Too many password to remember. Need a solution to store passwords
 - Who is the user?
	 Someone who is familiar with terminal. 
 - Why would someone use this?
	 - Local 
	 - Offline
	 - Automatic clipboard copy
 - What does success look like?
	 Project completion

### Functional Requirements
- Must-have features (V1/MVP)
	- Master password for authentication
	- Store Login credential (Service, username, password) e.g - Github - alan - asdfl;jdsl 
	- Password encryption
	- Clipboard copy
- Nice-to-have features
	- Password generation
	- Import Export
- Features intentionally postponed
- Project boundaries
 
### Non-functional Requirements (Qualities of the solution/project)
These describe qualities.
- Offline
- Single user
- password manager
- Windows CLI
- SQ Lite
- Master password
- Recovery key

### Data Model
Define the application's entities, their attributes, relationships, and constraints.
- password table (id, Service NOT NULL, username NOT NULL, password NOT NULL) 
- master password table NOT NULL

### Storage (Where data lives)
- Database (where structured data lives like username, password)
	- Relational database - sql
- Configuration (setting file which controls the application behavior)
- Persistence Strategy (When the data gets stored?)
	- Manual save
	- Save on exit

### High-Level Design (Architecture)
User -> CLI -> Command Dispatcher -> Auth/Vault/Generator -> Repository -> SQLite DB


### File structure:

src/
    main.c
    cli.c
	dispatcher.c
    auth.c
    vault.c
    repository.c
    crypto.c
    database.c
include/
    cli.h
	dispatcher.h
	auth.h
	vault.h
	database.h
	repository.h
	crypto.h
	models.h
crypton.db

### db design

VAULT_ENTRIES
  id
  site
  username
  password
  notes
  updated_at
  created_at

APP_METADATA
 master_password_hash
 recovery_key_hash
 updated_at
 created_at

 

### Module Design
1. main.c

Responsibility:-
 entry point of program.
 starting of application.
 handovering to CLI.
 handling the Exit.

 functions:- 

 int main(void);

2. cli.c

Responsibility:-
interacting to user.

this file will only handle i/o.

  Welcome screen
  Login prompt
  Command prompt
  User input
  Output print

functions:-

void start_cli(void);

void show_welcome(void);

void show_shell(void);

void read_command(char *buffer);

3. dispatcher.c

Responsibility:-

identifying the command and sending to correct module.

user -> add -> dispatcher -> vault_add()

functions:-

void dispatch_command(const char *command);


### Performance

- Expected dataset size
- Memory usage
- CPU usage
- Startup time
- Bottlenecks
### Error Handling

- Invalid input
- Missing files
- Corrupted data
- Network failure
- Recovery strategy
### Security

- Authentication
- Authorization
- Encryption
- Input validation
### Deployment

- Build process
- Packaging
- Installation
- Updates

### Technology Decisions
- Programming language
- Framework
- Database
- Libraries
- Build system
