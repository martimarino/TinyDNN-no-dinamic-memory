#pragma once
namespace tiny_dnn {
	namespace residual {
		std::vector<layer*> make_block2(size_t inputWidth,size_t inputHeight,size_t inputChannels,bool sub) {
			using sc = tiny_dnn::shortcut_layer;
			using conv = tiny_dnn::convolutional_layer;
			using relu = tiny_dnn::relu_layer;
			std::vector<layer*> residualBlocks;
			residualBlocks.push_back(new sc(tiny_dnn::ShortcutSide::BEGIN,inputWidth,inputHeight,inputChannels));
			
			size_t stride = sub?2:1;
			size_t outputChannels = inputChannels*((sub)?2:1);
			residualBlocks.push_back(
				new conv(inputWidth,inputHeight,3,inputChannels,outputChannels,padding::same,true, stride, stride, stride, stride)
			);

			residualBlocks.push_back(new relu());
			inputWidth/=(sub)?2:1;
			inputHeight/=(sub)?2:1;
			residualBlocks.push_back(
				new conv(inputWidth,inputHeight,3,outputChannels,outputChannels,padding::same,true, 1, 1, 1, 1)
			);

			residualBlocks.push_back(new sc(tiny_dnn::ShortcutSide::END,inputWidth,inputHeight,outputChannels));
			
			return residualBlocks;
		}

	}
}
