/////////////////////////////////////////////////////////////////////////////////
// File : Engine/Console/ConsoleCommand.inl
/////////////////////////////////////////////////////////////////////////////////
// Version : 1.0a
// Began Code : 29/05/2010
// Status : Alpha
// Portability : Any
/////////////////////////////////////////////////////////////////////////////////
// Description : Base class for commands & queries abstraction layer ...
/////////////////////////////////////////////////////////////////////////////////
// Part of Scarab-Engine, licensed under the
// Creative Commons Attribution-NonCommercial-NoDerivs 3.0 Unported License
//   http://creativecommons.org/licenses/by-nc-nd/3.0/
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Known Bugs : None
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// ConsoleCommandArgument implementation
inline Void ConsoleCommandArgument::AddType( ConsoleCommandArgType iType ) {
    iTypes |= iType;
}
inline Void ConsoleCommandArgument::RemoveType( ConsoleCommandArgType iType ) {
    iTypes &= ~iType;
}

inline Bool ConsoleCommandArgument::IsOptional() const {
    return ( (iFlags & CONSOLECOMMAND_ARG_OPTIONAL) != 0 );
}
inline Void ConsoleCommandArgument::SetOptional( Bool bOptional ) {
    if ( bOptional )
        iFlags |= CONSOLECOMMAND_ARG_OPTIONAL;
    else
        iFlags &= ~CONSOLECOMMAND_ARG_OPTIONAL;
}

inline Bool ConsoleCommandArgument::IsMutex() const {
    return ( (iFlags & CONSOLECOMMAND_ARG_MUTEX) != 0 );
}
inline UInt ConsoleCommandArgument::GetMutexCount() const {
    return (UInt)( iFlags & 0x000000ff );
}
inline Void ConsoleCommandArgument::SetMutex( UInt iCount ) {
    iFlags &= 0xffffff00;
    if ( iCount != INVALID_OFFSET ) {
        iFlags |= ( iCount & 0x000000ff );
        iFlags |= CONSOLECOMMAND_ARG_MUTEX;
    } else
        iFlags &= ~CONSOLECOMMAND_ARG_MUTEX;
}

/////////////////////////////////////////////////////////////////////////////////
// ConsoleCommand implementation
inline ConsoleCommandCategory ConsoleCommand::GetCategory() const {
    return m_iCategory;
}
inline const GChar * ConsoleCommand::GetName() const {
    return m_strName;
}

inline ConsoleCommandLevel ConsoleCommand::GetLevel() const {
    return m_iLevel;
}

inline UInt ConsoleCommand::GetMinArgCount() const {
    return m_iMinArgCount;
}
inline UInt ConsoleCommand::GetMaxArgCount() const {
    return m_iMaxArgCount;
}

inline UInt ConsoleCommand::GetArgCount() const {
    return m_iArgCount;
}
inline const ConsoleCommandArgument * ConsoleCommand::GetArgument( UInt iArgument ) const {
    Assert( iArgument < m_iArgCount );
    return ( m_arrArguments + iArgument );
}

