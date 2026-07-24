### Goals
 - What problem am I solving?
	 Too many password to remember. Need a solution to store passwords.
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
- Components 
	- What are the major parts of the system?
- Responsibility 
	- What is each component responsible for?
- Communication 
	- Who can communicate with whom?
- External Systems 
	- What external services or software does the system depend on? 
- Constraints 
	- What architectural constraints must the system satisfy?

### Module Design
For each module:

1. Responsibility
   - What is this module responsible for?
1. Owned Data
   - What data does this module own and manage?
1. Public API
   - What operations does it expose to other modules?
1. Dependencies
   - Which other modules does it depend on?
### Algorithms & Data Structures

- Data structures
- Search
- Sorting
- Parsing
- Indexing
- Complexity
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
