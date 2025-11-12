
# Potential Memory Leak !!!!

In this current design of the ECS system there is a memory leak in 
destructor of Archetype Manager where we only free archetype chunk as 
whole to which ever allocator we got but since our ECS components are not 
simple POD(Plain ol  Data) we need to call destructor of each components in the 
archetype chunk individually , this would require more complex destructor


# Bugs that cropped up during Testing

- Archetype Manager needs way to handle archetypeSignature with 
  zero set bits , this casued error when trying to access the component
  layouts in archetype definition for this zero signature.
