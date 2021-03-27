#pragma once
#include "ofParameter.h"
#include "ofPolyline.h"
#include "ofxCvGui/Utils/Sugar.h"
#include "ofxCeres.h"

namespace SA = ofxCeres::Models::StructuralAnalysis;

namespace Data
{
	struct StewartPlatform : ofParameterGroup, ofNode {
		struct Deck : ofParameterGroup, SA::System::Body {
			ofParameter<float> diameter{
				"Diameter [m]"
				, 1
				, 0.01
				, 5
			};

			ofParameter<float> jointSpacing{
				"Join spacing [m]"
				, 0.1
				, 0
				, 1
			};

			ofxLiquidEvent<void> onChange;

			Deck();
			void update();
			void markDirty();
		protected:
			void rebuild();
			ofEventListener diameterChangeListener;
			ofEventListener jointSpacingChangeListener;

			bool isDirty = true;
			float jointAngleOffset = 0; // angle between joint and coincident point
			ofPolyline line;
		};

		struct Actuators : ofParameterGroup {
			ofParameter<float> minimumLength{
				"Minimum length [m]"
				, 0.4
				, 0.01
				, 5
			};

			ofParameter<float> maximumLength{
				"Maximum length [m]"
				, 0.8
				, 0.01
				, 5
			};

			struct Actuator : SA::System::Body
			{
				Actuator();

				ofParameter<float> value;
			};
			std::shared_ptr<Actuator> actuators[6];

			ofxLiquidEvent<void> onValueChange;
			
			Actuators();

			ofEventListener maxChangeListener;
			ofEventListener minChangeListener;
			ofEventListener valueChangeListener[6];
		} actuators;

		struct : ofParameterGroup
		{
			ofParameter<float> offset{ "Offset [m]", 0.5f, -1.0f, 1.0f };
			ofParameter<float> mass{ "Mass [kg]", 150, 0, 300 };

			bool isDirty = true;
			PARAM_DECLARE("Weight", offset, mass);
		} weight;

		StewartPlatform();
		void update();
		void customDraw() override;
		
		void markDirty();

		void solveForces();

		SA::System system;

		std::shared_ptr<Deck> upperDeck;
		std::shared_ptr<Deck> lowerDeck;

	protected:
		void rebuild();
		void rebuildWeight();
		bool isDirty = true;

		struct {
			float jointAngleOffsetUpper; /// offset from joints coinciding in radians
			float jointAngleOffsetLower; /// offset from joints coinciding in radians
		} cached;
	};
}