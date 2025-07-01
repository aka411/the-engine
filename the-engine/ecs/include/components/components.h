#pragma once


namespace TheEngine::ECS
{
  

        struct TransformComponent
        {
            glm::vec3 position = glm::vec3(0.0f);
            glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            glm::vec3 scale = glm::vec3(1.0f);
        };


        struct RenderableComponent
        {
            MeshComponent mesh;

            MaterialComponent material;


            bool isVisible = true;
            // Optional: Flags or properties related to rendering
            bool castsShadows = true;
           // bool receivesShadows = true;
            // ... other rendering properties
        };


        struct PhysicsComponent
        {
            bool is_static = false;
            float mass = 1.0f;
            glm::vec3 velocity = glm::vec3(0.0f);
            glm::vec3 angular_velocity = glm::vec3(0.0f);
            glm::vec3 force = glm::vec3(0.0f);
            glm::vec3 torque = glm::vec3(0.0f);
            float linear_damping = 0.0f;
            float angular_damping = 0.0f;
            float friction = 0.5f;
            float restitution = 0.2f;
            bool is_awake = true;


            // Center of mass offset in local space
            glm::vec3 centerOfMassOffset = glm::vec3(0.0f);

            // ... other physics properties (inertia tensor, etc.)
        };





    
} // namespace TheEngine::ECS


