### Tasks

- [x] database -> rupak
- [x] cli, crpyto -> ankita
- [ ] repository -> prolay
- [ ] parser -> sankhadigop
      **DEADLINE:** 28.07.2026 10:00 am
- [ ] vault -> rupak

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
cli.c -> done
parser.c -> done
auth.c
vault.c
repository.c -> done
crypto.c -> done
database. -> done
include/
cli.h -> done
parser.h -> done
auth.h
vault.h
repository.h -> done
crypto.h -> done
database.h -> done
model.h -> data model -> done
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

void read_command(char \*buffer);

3. parser.c

Responsibility:-

identifying the command and sending to correct module.

user -> add -> parser -> vault_add()

functions:-

void parser_command(const char \*command);

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

bool verify_master_password(const char \*password);

bool verify_recovery_key(const char \*key);

5. repository.c
   Responsibility:-
   Acts as the data access layer between the application and the database.
   Performs CRUD (Create, Read, Update, Delete) operations.
   Converts database records into application models and vice versa.
   Hides SQL implementation details from the business logic.

functions:-
bool save_entry(VaultEntry \*entry);

bool update_entry(VaultEntry \*entry);

bool delete_entry(char \*service);

VaultEntry get_entry_by_service(char \*service);

int find_entries_by_site(const char \*service, VaultEntryList results);

int get_all_entries(VaultEntryList results);

bool save_metatdata(AppMetadata metadata)

bool load_metadata(AppMetadata *matadata)

bool update_metadata(AppMetadata metadata)

6. database.c
   Responsibility:-
   Manages SQLite database connections.
   Initializes the database.
   Creates required tables during first-time setup.
   Opens and closes database connections.
   Executes low-level database operations.

functions:-

int open_database();

int create_table();

int add_entry(VaultEntry entry);

VaultEntryList get_all_entries();

int close_database();

7. crypto.c
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

8. models.h
   Responsibility:-
   Defines shared data structures used throughout the application.
   Represents database entities as C structures.
   Provides a common interface for data exchange between modules.

structure:-
typedef struct date_data
{
int day;
int month;
int year;
} date;
typedef struct
{
...
} VaultEntry;

typedef struct
{
...
} AppMetadata;

9. vault.c

 responsibilities:-

 Acts as the business logic layer between the CLI and repository.
Validates user input before processing.
Encrypts passwords before storing them.
Decrypts passwords after retrieving them.
Calls the Repository layer for CRUD operations.
Generates secure random passwords.
Does not perform database operations directly.
Does not interact with the user interface.

functions:-
bool vault_add_entry(VaultEntry entry);

bool vault_update_entry(VaultEntry entry);

bool vault_delete_entry(char *service);

bool vault_get_entry(int id, VaultEntry *entry);

int vault_find_entries(const char *service,
                       VaultEntry results[],
                       int max_results);

VaultEntryList vault_get_all_entries(void);

void vault_generate_password(char *password, int length);


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

- CMAKE for executable

### Technology Decisions

- Programming language - C
- Database - SQLite
- Libraries - string
