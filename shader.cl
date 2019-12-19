__kernel void func(global float4* color, int width, int height, int time)
{
	int gid = get_global_id(0);
        float2 uv = (float2)((float)(gid % width) / width, (float)(gid / width) / height);
	color[gid] = (float4)(uv.x, 0, uv.y, 1.0f);
}