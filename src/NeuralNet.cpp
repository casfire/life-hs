#include "NeuralNet.hpp"

#if _NEURALNETWORK_VER_ != 114
	#warning Invalid NeuralNet header version
#endif

/* NeuralNet */

NeuralNet::NeuralNet(){
	weights.clear();
	nodes.clear();
	nodeRnd.clear();
	nodeCount = 0;
}

NeuralNet::~NeuralNet(){
	Clear();
}

NeuralNet& NeuralNet::operator = (const NeuralNet &param){
	if(this==&param){
		return *this;
	}
	SetSize(param.nodeCount);
	for(unsigned int n=0; n<nodeCount; n++){
		nodes.at(n) = param.nodes.at(n);
		nodeRnd.at(n) = param.nodeRnd.at(n);
	}
	for(unsigned int n=0; n<nodeCount; n++){
		for(unsigned int k=0; k<nodeCount; k++){
			weights.at(n).at(k) = param.weights.at(n).at(k);
		}
	}
	return *this;
}

void NeuralNet::Step(){

	//sum junction
	for(unsigned int n=0; n<nodeCount; n++){
		double t=0., b=0., res;
		for(unsigned int k=0; k<nodeCount; k++){
			t+=weights.at(n).at(k)*nodes.at(k).val;
			b+=weights.at(n).at(k);
		}
		if(b>0.)res=t/b;
		else res = 0.;
		nodes.at(n).sum=res;
	}

	//activation function
	for(unsigned int n=0; n<nodeCount; n++){
		nodes.at(n).Activate();
	}

}

void NeuralNet::SetSize(unsigned int size){
	unsigned int c = nodeCount;
	nodeCount = size;
	nodes.resize(size);
	for(unsigned int n=0; n<c; n++){
		if(n>=size)weights.at(n).clear();
		else{
			nodeRnd.at(n) = n;
			weights.at(n).resize(size);
		}
	}
	nodeRnd.resize(size);
	weights.resize(size);
	for(unsigned int n=c; n<size; n++){
		nodes.at(n).Construct();
		weights.at(n).resize(size);
		nodeRnd.at(n) = n;
		for(unsigned int k=0; k<size; k++){
			weights.at(n).at(k) = NEURALNET_DEFAULTWEIGHT;
		}
	}
}

unsigned int NeuralNet::GetSize(){
	return nodeCount;
}

void NeuralNet::Clear(){
	for(unsigned int n=0; n<weights.size(); n++)weights.at(n).clear();
	weights.clear();
	nodes.clear();
	nodeRnd.clear();
	nodeCount = 0;
}

void NeuralNet::SetNode(unsigned int node, float value){
	if(node >= nodeCount)return;
	nodes.at(node).val = BOUNDSNODEVAL(value);
}

void NeuralNet::SetNodeAll(float value){
	value = BOUNDSNODEVAL(value);
	for(unsigned int n=0; n<nodeCount; n++){
		nodes.at(n).val = value;
	}
}

void NeuralNet::SetNodesRandom(Random *rnd, float min, float max){
	if(rnd == NULL)rnd = &randomGenerator;
	for(unsigned int n=0; n<nodeCount; n++){
		nodes.at(n).val = rnd->xrand(min,max);
		nodes.at(n).val = BOUNDSNODEVAL(nodes.at(n).val);
	}
}

float NeuralNet::GetNode(unsigned int node){
	if(node >= nodeCount)return 0.;
	return nodes.at(node).val;
}

void NeuralNet::SetWeight(unsigned int from, unsigned int to, float weight){
	if((from >= nodeCount) || (to >= nodeCount))return;
	weights.at(to).at(from) = BOUNDSWEIGHT(weight);
}

void NeuralNet::SetWeightAll(float weight){
	weight = BOUNDSWEIGHT(weight);
	for(unsigned int n=0; n<nodeCount; n++){
		for(unsigned int k=0; k<nodeCount; k++){
			weights.at(n).at(k) = weight;
		}
	}
}

void NeuralNet::SetWeightsRandom(Random *rnd, float min, float max){
	if(rnd == NULL)rnd = &randomGenerator;
	for(unsigned int n=0; n<nodeCount; n++){
		for(unsigned int k=0; k<nodeCount; k++){
			weights.at(n).at(k) = rnd->xrand(min,max);
			weights.at(n).at(k) = BOUNDSWEIGHT(weights.at(n).at(k));
		}
	}
}

float NeuralNet::GetWeight(unsigned int from, unsigned int to){
	if((from >= nodeCount) || (to >= nodeCount))return 0.;
	return weights.at(to).at(from);
}

void NeuralNet::SetParams(unsigned int node, float ma, float mb, float p){
	if(node >= nodeCount)return;
	nodes.at(node).SetParams(ma,mb,p);
}

void NeuralNet::SetParamsAll(float ma, float mb, float p){
	for(unsigned int n=0; n<nodeCount; n++)nodes.at(n).SetParams(ma,mb,p);
}

void NeuralNet::SetParamsRandom(Random *rnd){
	if(rnd == NULL)rnd = &randomGenerator;
	for(unsigned int n=0; n<nodeCount; n++){
		nodes.at(n).SetParamsRandom(rnd);
	}
}

void NeuralNet::GetParams(unsigned int node, float *ma, float *mb, float *p){
	if(node >= nodeCount)return;
	nodes.at(node).GetParams(ma,mb,p);
}

void NeuralNet::Mutate(Random *rnd, float size, float div){
	if(rnd == NULL)rnd = &randomGenerator;
	for(unsigned int n = nodeCount; n-- > 1;){
		unsigned int r = rnd->Next()%n;
		unsigned int t = nodeRnd.at(n);
		nodeRnd.at(n) = nodeRnd.at(r);
		nodeRnd.at(r) = t;
	}

	unsigned int nodeCountEnd = nodeCount - 1, nodeCount2 = nodeCount*nodeCount, nodeCount3 = nodeCount*3;
	float sum = nodeCount3*size, sumW = nodeCount2*size;
	float max = 1., maxW = 1.;
	float min = 0., minW = 0.;
	float b1,b2;
	for(unsigned int n=0; n<nodeCount; n++){

		float n1, n2, n3, r;

		if(sum < max)max = sum;
		r = sum-nodeCount3+(n*3)+1;
		if(r > min)min = r;
		b1 = min+(size-min)*div; b2 = max+(size-max)*div;
		n1 = rnd->xrand(BOUNDS(b1,min,max),BOUNDS(b2,min,max));
		sum -= n1;

		if(sum < max)max = sum;
		r = sum-nodeCount3+(n*3)+2;
		if(r > min)min = r;
		b1 = min+(size-min)*div; b2 = max+(size-max)*div;
		n2 = rnd->xrand(BOUNDS(b1,min,max),BOUNDS(b2,min,max));
		sum -= n2;

		if(sum < max)max = sum;
		r = sum-nodeCount3+(n*3)+3;
		if(r > min)min = r;
		if(n == nodeCountEnd){
			n3 = sum;
		}else{
			b1 = min+(size-min)*div; b2 = max+(size-max)*div;
			n3 = rnd->xrand(BOUNDS(b1,min,max),BOUNDS(b2,min,max));
			sum -= n3;
		}

		unsigned int i1 = nodeRnd.at(n);
		unsigned int i2 = nodeRnd.at(i1);
		unsigned int i3 = nodeRnd.at(i2);

		nodes.at(i1).ma += rnd->xrand(-n1,n1);
		nodes.at(i2).mb += rnd->xrand(-n2,n2);
		nodes.at(i3).p += rnd->xrand(-n3,n3);

		nodes.at(i1).ma = BOUNDS(nodes.at(i1).ma,-1.,1.);
		nodes.at(i2).mb = BOUNDS(nodes.at(i2).mb,-1.,1.);
		nodes.at(i3).p = BOUNDS(nodes.at(i3).p,-1.,1.);

		for(unsigned int k=0; k<nodeCount; k++){
			if(sumW < maxW)maxW = sumW;
			float rW = sumW-nodeCount2+(n*k)+1;
			if(rW > minW)minW = rW;

			float num;
			if((k == nodeCountEnd) && (n == nodeCountEnd)){
				num = sumW;
			}else{
				b1 = minW+(size-minW)*div; b2 = maxW+(size-maxW)*div;
				num = rnd->xrand(BOUNDS(b1,minW,maxW),BOUNDS(b2,minW,maxW));
				sumW -= num;
			}

			unsigned int w1 = nodeCountEnd - i3;
			unsigned int w2 = nodeRnd.at(k);
			w1 += w2; if(w1 >= nodeCount)w1 -= nodeCount;
			w2 += w1; if(w2 >= nodeCount)w2 -= nodeCount;
			w1 = nodeRnd.at(w1);
			w2 = nodeRnd.at(w2);

			float wx1,wx2;
			if(num >= 0.5){
				wx1 = 0.;
				wx2 = 1.;
			}else{
				wx1 = weights.at(w1).at(w2) - num;
				wx2 = weights.at(w1).at(w2) + num;
				if(wx1 < 0.){ wx2 -= wx1; wx1 = 0.;}
				if(wx2 > 1.){ wx1 += wx2-1.; wx2 = 1.; }
			}
			weights.at(w1).at(w2) = rnd->xrand(wx1,wx2);
			//weights.at(w1).at(w2) += rnd->xrand(-num,num);
			weights.at(w1).at(w2) = BOUNDSWEIGHT(weights.at(w1).at(w2));
		}

	}

}

void NeuralNet::Crossover(NeuralNet *A, NeuralNet *B){
	SetSize((A->GetSize()+B->GetSize())/2);
	for(unsigned int n=0; n<nodeCount; n++){
		float a1, a2, b1, b2, p1, p2;
		A->GetParams(n,&a1,&b1,&p1);
		B->GetParams(n,&a2,&b2,&p2);
		nodes.at(n).SetParams((a1+a2)/2.,(b1+b2)/2.,(p1+p2)/2.);
		nodes.at(n).val = 0.;
		for(unsigned int k=0; k<nodeCount; k++){
			weights.at(n).at(k) = (A->GetWeight(k,n)+B->GetWeight(k,n))/2.;
			weights.at(n).at(k) = BOUNDSWEIGHT(weights.at(n).at(k));
		}
	}
}

#define NEURALNET_HEADER {'n', 'e', 't', 0x10}
#define NEURALNET_HEADER_SIZE 4

bool NeuralNet::SaveToFile(const char *filename){
	if((filename == NULL) || (filename[0] == 0))return false;
	FILE *file = fopen(filename,"wb");
	if(file == NULL)return false;
	char header[] = NEURALNET_HEADER;
	if(fwrite((void*)header,NEURALNET_HEADER_SIZE,1,file) != 1){ fclose(file); return false; }
	if(fwrite((void*)(&nodeCount),sizeof(unsigned int),1,file) != 1){ fclose(file); return false; }
	for(unsigned int n=0; n<nodeCount; n++){
		if(fwrite((void*)(&(nodes.at(n))),sizeof(NeuralNode),1,file) != 1){ fclose(file); return false; }
		if(fwrite((void*)(&(nodeRnd.at(n))),sizeof(unsigned int),1,file) != 1){ fclose(file); return false; }
	}
	for(unsigned int n=0; n<nodeCount; n++){
		for(unsigned int k=0; k<nodeCount; k++){
			if(fwrite((void*)(&(weights.at(n).at(k))),sizeof(float),1,file) != 1){ fclose(file); return false; }
		}
	}
	fclose(file);
	return true;
}

bool NeuralNet::LoadFromFile(const char *filename){
	if((filename == NULL) || (filename[0] == 0))return false;
	FILE *file = fopen(filename,"rb");
	if(file == NULL)return false;
	char header[] = NEURALNET_HEADER;
	char h[NEURALNET_HEADER_SIZE];
	if(fread((void*)h,NEURALNET_HEADER_SIZE,1,file) != 1){ fclose(file); return false; }
	for(unsigned int n=0; n<NEURALNET_HEADER_SIZE; n++){
		if(h[n] != header[n]){ fclose(file); return false; }
	}
	unsigned int size;
	if(fread((void*)(&size),sizeof(unsigned int),1,file) != 1){ fclose(file); return false; }
	SetSize(size);
	for(unsigned int n=0; n<size; n++){
		if(fread((void*)(&(nodes.at(n))),sizeof(NeuralNode),1,file) != 1){ fclose(file); SetSize(0); return false; }
		if(fread((void*)(&(nodeRnd.at(n))),sizeof(unsigned int),1,file) != 1){ fclose(file); SetSize(0); return false; }
	}
	for(unsigned int n=0; n<size; n++){
		for(unsigned int k=0; k<size; k++){
			if(fread((void*)(&(weights.at(n).at(k))),sizeof(float),1,file) != 1){ fclose(file); SetSize(0); return false; }
		}
	}
	fclose(file);
	return true;
}

#ifdef _CONFIGFILE_HPP_

bool NeuralNet::ConfigSet(ConfigFile *cfg, const char *prefix){
	if((cfg == NULL) || ((prefix != NULL) && (strlen(prefix) > 200)))return false;
	char name[256];
	bool ret = true;
	sprintf(name,"%s_netNodeCount",((prefix==NULL)?"":prefix));
	if(!cfg->Set(name,nodeCount))ret = false;
	sprintf(name,"%s_netNodeRnd",((prefix==NULL)?"":prefix));
	if(!cfg->SetArray(name,(unsigned int *)(&nodeRnd[0]),nodeRnd.size()))ret = false;
	std::vector<float> tmp; tmp.clear();
	for(unsigned int n=0; n<nodeCount; n++){
		tmp.push_back(nodes.at(n).ma);
		tmp.push_back(nodes.at(n).mb);
		tmp.push_back(nodes.at(n).p);
		tmp.push_back(nodes.at(n).sum);
		tmp.push_back(nodes.at(n).val);
	}
	sprintf(name,"%s_netNodes",((prefix==NULL)?"":prefix));
	if(!cfg->SetArray(name,(float*)(&tmp[0]),nodeCount*5))ret = false;
	tmp.clear();
	for(unsigned int n=0; n<nodeCount; n++){
		for(unsigned int k=0; k<nodeCount; k++){
			tmp.push_back(weights.at(n).at(k));
		}
	}
	sprintf(name,"%s_netWeights",((prefix==NULL)?"":prefix));
	if(!cfg->SetArray(name,(float*)(&tmp[0]),nodeCount*nodeCount))ret = false;
	tmp.clear();
	return ret;
}

bool NeuralNet::ConfigLoad(ConfigFile *cfg, const char *prefix){
	if((cfg == NULL) || ((prefix != NULL) && (strlen(prefix) > 200)))return false;
	char name[256];
	NeuralNet tnet; tnet.Construct();
	std::vector<float> tmp; tmp.clear();
	unsigned int nc;
	sprintf(name,"%s_netNodeCount",((prefix==NULL)?"":prefix));
	if(!cfg->Get(name,&nc)){ tnet.Destruct(); return false; }
	tnet.SetSize(nc);
	sprintf(name,"%s_netNodeRnd",((prefix==NULL)?"":prefix));
	if(!cfg->GetArray(name,((unsigned int*)(&tnet.nodeRnd[0])),&nc,tnet.nodeCount)){ tnet.Destruct(); return false; }
	if(nc != tnet.nodeCount){ tnet.Destruct(); return false; }
	tmp.resize(tnet.nodeCount*5);
	sprintf(name,"%s_netNodes",((prefix==NULL)?"":prefix));
	if(!cfg->GetArray(name,((float*)(&tmp[0])),&nc,tnet.nodeCount*5)){ tnet.Destruct(); return false; }
	if(nc != tnet.nodeCount*5){ tnet.Destruct(); return false; }
	for(unsigned int n=0; n<tnet.nodeCount; n++){
		tnet.nodes.at(n).ma =  tmp[(n*5)+0];
		tnet.nodes.at(n).mb =  tmp[(n*5)+1];
		tnet.nodes.at(n).p =   tmp[(n*5)+2];
		tnet.nodes.at(n).sum = tmp[(n*5)+3];
		tnet.nodes.at(n).val = tmp[(n*5)+4];
	}
	tmp.resize(tnet.nodeCount*tnet.nodeCount);
	sprintf(name,"%s_netWeights",((prefix==NULL)?"":prefix));
	if(!cfg->GetArray(name,((float*)(&tmp[0])),&nc,tnet.nodeCount*tnet.nodeCount)){ tnet.Destruct(); return false; }
	if(nc != tnet.nodeCount*tnet.nodeCount){ tnet.Destruct(); return false; }
	for(unsigned int n=0; n<tnet.nodeCount; n++){
		for(unsigned int k=0; k<tnet.nodeCount; k++){
			tnet.weights.at(n).at(k) = tmp[(n*tnet.nodeCount)+k];
		}
	}
	*this = tnet;
	tnet.Destruct();
	return true;
}

#endif

#ifdef _SDL_gfxPrimitives_h
void NeuralNet::Draw(SDL_Surface *dst, int posX1, int posY1, int posX2, int posY2){
	float spaceX = (float)(posX2-posX1)/nodeCount;
	float spaceY = (float)(posY2-posY1)/nodeCount;
	for(unsigned int x = 0; x<nodeCount; x++){
		for(unsigned int y = 0; y<nodeCount; y++){
			int px1 = posX1 + x*spaceX, px2 = px1 + spaceX;
			int py1 = posY1 + y*spaceY, py2 = py1 + spaceY;
			float w = 1.-weights[x][y], v = nodes[(x < y)?y:x].val;
			Uint8 r,g,b,a = 0xFF;
			if(v < 0.){
				r = (176*w)+(-v*79);
				g = 176*w;
				b = 176*w;
			}else{
				r = 176*w;
				g = 176*w;
				b = (176*w)+(v*79);
			}
			boxRGBA(dst,px1,py1,px2,py2,r,g,b,a);
		}
	}
	rectangleColor(dst,posX1,posY1,posX2,posY2,0x000000FF);
}
#endif

/* NeuralNode */

NeuralNode::NeuralNode(){
	Construct();
}

NeuralNode::NeuralNode(float value){
	Construct();
	val = BOUNDSNODEVAL(value);
}

void NeuralNode::Construct(){
	sum = 0.;
	val = NEURALNODE_DEFAULTVAL;
	ma = 0.;
	mb = 0.;
	p = 0.;
}

void NeuralNode::Activate(){
	if(mb == 0.)val = p;
	else{
		val = tanh(sum*(ma/mb))+p;
		//val = (1.-s)*tanh((ma/mb)*(sum+p)) + s*(ma/mb)*(sum+p);
		val = BOUNDSNODEVAL(val);
	}
}

void NeuralNode::SetParams(float vma, float vmb, float vp){
	ma = BOUNDS(vma,-1.,1.);
	mb = BOUNDS(vmb,-1.,1.);
	p	= BOUNDS(vp ,-1.,1.);
}

void NeuralNode::SetParamsRandom(Random *rnd){
	if(rnd==NULL)rnd = &randomGenerator;
	ma = rnd->xrand(-1.,1.);
	mb = rnd->xrand(-1.,1.);
	p =	rnd->xrand(-1.,1.);
}

void NeuralNode::GetParams(float *vma, float *vmb, float *vp){
	*vma = ma;
	*vmb = mb;
	*vp = p;
}
