<patch appVersion="2.0.0">
   <obj type="patch/inlet a" uuid="b577fe41e0a6bc7b5502ce33cb8a3129e2e28ee5" name="in" x="42" y="308">
      <params/>
      <attribs/>
   </obj>
   <obj type="env/follower" uuid="356ce1d18ac6b51704833f94dac5dea823fb8223" name="follower_1" x="126" y="308">
      <params/>
      <attribs>
         <combo attributeName="time" selection="170.6ms"/>
      </attribs>
   </obj>
   <patchobj type="math/&gt;c" uuid="99f2934d97d62b715a829979ef6c8abef35dcdb2" name="Treshold" x="238" y="308">
      <params>
         <frac32.u.map name="c" onParent="true" value="0.0">
            <modulators/>
            <presets/>
         </frac32.u.map>
      </params>
      <attribs/>
      <object id="patch/object" uuid="99f2934d97d62b715a829979ef6c8abef35dcdb2" sha="ccb67d0fb40e8b7fa154eb9720c21fa4e1d24836">
         <upgradeSha>aa245f90aec358415dbbc12409c90065cda73d3e</upgradeSha>
         <sDescription>greater than constant</sDescription>
         <author>Johannes Taelman</author>
         <license>BSD</license>
         <helpPatch>math.axh</helpPatch>
         <inlets>
            <frac32 name="in" description="in"/>
         </inlets>
         <outlets>
            <bool32 name="out" description="out"/>
         </outlets>
         <displays/>
         <params>
            <frac32.u.map name="c" noLabel="true">
               <DefaultValue v="0.0"/>
            </frac32.u.map>
         </params>
         <attribs/>
         <file-depends/>
         <includes/>
         <depends/>
         <modules/>
         <code.declaration><![CDATA[]]></code.declaration>
         <code.init><![CDATA[]]></code.init>
         <code.dispose><![CDATA[]]></code.dispose>
         <code.krate><![CDATA[outlet_out = inlet_in > param_c>>4;]]></code.krate>
         <code.srate><![CDATA[]]></code.srate>
         <code.midihandler><![CDATA[]]></code.midihandler>
      </object>
   </patchobj>
   <obj type="env/ahd" uuid="2139369d6de2ba5ddf76e602d1d25df653ca9eed" name="envelope" x="350" y="308">
      <params>
         <frac32.s.map name="a" onParent="true" value="0.0">
            <modulators/>
            <presets/>
         </frac32.s.map>
         <frac32.s.map name="d" onParent="true" value="0.0">
            <modulators/>
            <presets/>
         </frac32.s.map>
      </params>
      <attribs/>
   </obj>
   <obj type="gain/vca" uuid="a9f2dcd18043e2f47364e45cb8814f63c2a37c0d" name="vca_1" x="462" y="392">
      <params/>
      <attribs/>
   </obj>
   <obj type="patch/outlet a" uuid="abd8c5fd3b0524a6630f65cad6dc27f6c58e2a3e" name="out" x="546" y="392">
      <params/>
      <attribs/>
   </obj>
   <nets>
      <net>
         <source obj="in" outlet="inlet"/>
         <dest obj="follower_1" inlet="in"/>
         <dest obj="vca_1" inlet="a"/>
      </net>
      <net>
         <source obj="envelope" outlet="env"/>
         <dest obj="vca_1" inlet="v"/>
      </net>
      <net>
         <source obj="vca_1" outlet="o"/>
         <dest obj="out" inlet="outlet"/>
      </net>
      <net>
         <source obj="follower_1" outlet="amp"/>
         <dest obj="Treshold" inlet="in"/>
      </net>
      <net>
         <source obj="Treshold" outlet="out"/>
         <dest obj="envelope" inlet="gate"/>
      </net>
   </nets>
   <settings>
      <subpatchmode>no</subpatchmode>
   </settings>
   <notes><![CDATA[]]></notes>
   <windowPos>
      <x>36</x>
      <y>23</y>
      <width>1382</width>
      <height>784</height>
   </windowPos>
</patch>