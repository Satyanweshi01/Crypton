### Tasks 
 - [ ] database -> rupak 
 - [ ] cli, crpyto -> ankita
 - [ ] repository -> prolay
 - [ ] parser -> sankhadeep
**DEADLINE:** 28.07.2026 10:00 am

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
	- Recovery key
	- Password history (to avoid rewrite previous password)
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
User -> CLI -> Command Parser -> Auth/Vault/Generator(Business logic) -> CRUD C layer -> SQLite DB


### File structure:

src/
    main.c
    cli.c
	parser.c
    auth.c
    vault.c
    repository.c
    crypto.c
    database.c
include/
    cli.h
	parser.h
	auth.h
	vault.h
	repository.h
	crypto.h
	database.h
	models.h -> data model
crypton.db

### db design

VAULT_ENTRIES
  id
  service
  username
  password
  notes
  updated_at
  created_at

APP_METADATA
 master_password_hash
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

3. parser.c

Responsibility:-

identifying the command and sending to correct module.

user -> add -> parser -> vault_add()

functions:-

void parser_command(const char *command);

4. auth.c
Responsibility:-
Handles vault authentication.
Creates the master password during first-time setup.
Verifies the master password during login.
Verifies the recovery key.
Manages vault initialization and access control.

functions:-

bool vault_exists(void);

void setup_vault(void);

bool login(void);

bool verify_master_password(const char *password);

bool verify_recovery_key(const char *key);

5. vault.c
Responsibility:-
Implements the core business logic of the password manager.
Handles password entry operations such as add, retrieve, update, delete, and list.
Coordinates with the repository layer for data persistence.
Uses the crypto module for encryption and decryption.

functions:-

void add_entry(void);

void list_entries(void);

void get_entry(void);

void update_entry(void);

void delete_entry(void);

void generate_password(void);

6. repository.c
Responsibility:-
Acts as the data access layer between the application and the database.
Performs CRUD (Create, Read, Update, Delete) operations.
Converts database records into application models and vice versa.
Hides SQL implementation details from the business logic.

functions:-
bool save_entry(VaultEntry *entry);

bool update_entry(VaultEntry *entry);

bool delete_entry(int id);

VaultEntry get_entry_by_id(int id);

int find_entries_by_site(const char *site, VaultEntry results[]);

int get_all_entries(VaultEntry results[]);

7. database.c
Responsibility:-
Manages SQLite database connections.
Initializes the database.
Creates required tables during first-time setup.
Opens and closes database connections.
Executes low-level database operations.

functions:-

bool init_database(void);

bool open_database(void);

void close_database(void);

bool create_tables(void);

8. crypto.c
Responsibility:-

Provides cryptographic services for the application.
Hashes and verifies the master password.
Encrypts and decrypts stored passwords.
Generates secure random passwords.
Keeps all cryptographic operations centralized.

functions:-
void hash_password(...);

bool verify_hash(...);

void encrypt(...);

void decrypt(...);

void generate_random_password(...);

9. models.h
Responsibility:-
Defines shared data structures used throughout the application.
Represents database entities as C structures.
Provides a common interface for data exchange between modules.

functions:-

typedef struct
{
    ...
} VaultEntry;

typedef struct
{
    ...
} AppMetadata;

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
