#pragma once

namespace TheEngine::Core
{
    // Identifier for game entities
    using EntityId = unsigned int; // belongs to ecs system only

    // Identifier for components attached to entities
    using ComponentId = unsigned int;

    using ResourceHandle = unsigned int; 

    // we can add other fundamental type aliases here in the future
    // For example:
    // using ResourceHandle = unsigned long;
    // enum class LogLevel { ... };

} // namespace TheEngine::Core

 