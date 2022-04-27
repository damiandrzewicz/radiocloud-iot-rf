# Software Design

sdsdvsdv

## Messaging Library

### Message Interface

@startuml 

interface IMessage
{
    + {abstract} bool parse(bool verify = true)
    + {abstract} bool build()
}

@enduml
