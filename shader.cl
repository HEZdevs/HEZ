__kernel void func(global float4* color, int width, int height, int time)
{
	int gid = get_global_id(0);
        float2 uv = (float2)((float)(gid % width) / width, (float)(gid / width) / height);
	color[gid] = (float4)(uv.x, cos(time * 0.01f) * 0.5f + 0.5f, uv.y, 1.0f);
}