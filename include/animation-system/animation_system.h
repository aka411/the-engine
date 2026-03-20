#pragma once


namespace TheEngine::AnimationSystem
{

	/*
	
	Need to design this after reading  on animation systems
	
	*/



	class AniamtionSystem
	{

	private:

		class AnimationResourceSystem;

		AnimationResourceSystem& m_animationResoruceSystem;


	public:

		AniamtionSystem(AnimationResourceSystem& animationResoruceSystem);

		//what all should this expose?
		//Expose most less to do the maximum and avoid coupling.

		//sample methods so as to just remember an idea of what to do, not the best methods
		//use std::span??
		void animateKeyframeAnimation(TransfomationComponent* transformationComponent,const int transfomationComponentCount,  AnimationState& animationState, float deltaTime);
		void animateSkeletalAnimation();



	};


}